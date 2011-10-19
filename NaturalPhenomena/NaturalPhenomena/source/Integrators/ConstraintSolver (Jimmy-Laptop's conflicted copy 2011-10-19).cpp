
#include <Integrators\ConstraintSolver.hpp>



ConstraintSolver::ConstraintSolver(Solver* solver):pSolver(solver){
	
	//Row = this->pSolver->_pConstraints.size();
	this->_isInitialized = false;
}

ConstraintSolver::~ConstraintSolver(){

	delete[] _J;
	delete[] _WeightInverse;
	
	delete[] _JDot;
	delete[] ParticleIDs;
	delete[] _qDot;
	delete[] _QForces;
	delete[] _Lamada;
}

void ConstraintSolver::Initialize()
{
	Row = this->pSolver->_pConstraints.size();
	Col = this->pSolver->_numOfParti * 3;
	
	//this->_leftHandSide_m = MatrixP(Row,Row);
	//this->_rightHandSide_m = MatrixP(Row,1);
	this->ParticleIDs = new int[Row];
	this->_J = new DATA[Row*Col];
	this->_WeightInverse = new DATA[Col];
	this->_JT = new DATA[Col*Row];
	this->_JDot = new DATA[Row*Col];
	this->_qDot = new DATA[Col*1];
	this->_QForces = new DATA[Col*1];
	this->_Lamada = new DATA[Row*1];
	
	for(int i=0;i<this->pSolver->_pConstraints.size();i++)
	{
		Particle* pParti = this->pSolver->_pConstraints[i]->m_p1;
		this->ParticleIDs[i]=pParti->ParticleID;
	}

	this->_isInitialized = true;

}
void ConstraintSolver::Solve()
{
	if(!_isInitialized)
		this->Initialize();

	
	
	//Fill Jacobian Matrix

	for(int row=0;row<Row;row++)
	{
		
		for(int j=0;j<this->pSolver->_numOfParti;j++)
		{
			for(int k=0;k<3;k++)
			{
				int col = (j*3)+k;
				Particle* pParti = this->pSolver->_pConstraints[row]->m_p1;

				this->_J[row*(Col)+col] = pParti->m_Position[k];
				this->_JDot[row*(Col)+col] = pParti->m_Velocity[k];
			}

			//Fill Jacobian Matrix
			if(pSolver->_pConstraints[row]->m_p1->ParticleID==j)
			{
				pSolver->_pConstraints[row]->gradient(&_J[row*(Col)+(j*3)]);
				pSolver->_pConstraints[row]->gradientDot(&_JDot[row*(Col)+(j*3)]);
			}
			else
				for(int k=0;k<3;k++)
				{
					int col = (j*3)+k;
					_J[row*(Col)+col] = 0;
					_JDot[row*(Col)+col] = 0;
				}
		}
	}
	for(int r = 0; r<Row;r++)
	{
		printf("\n_J");
		for(int c = 0; c<Col;c++)
		{
			printf(",");
			printf("%f",_J[r*Row+c]);
		}
		printf("\n_JDot");
		for(int c = 0; c<Col;c++)
		{
			printf(",");
			printf("%f",_JDot[r*Row+c]);
		}
	}
	
	//Fill _WeightInverse Matrix &
	for(int r=0;r<Col;r++)
	{
		int p =(r-r%3)/3;
		this->_WeightInverse[r]=1.0/this->pSolver->_pParti[p]->m_Mass;
		for(int i=0;i<3;i++)
		{
			_qDot[p*3+i]= pSolver->_pParti[p]->m_Velocity[i];
			_QForces[p*3+i] =  pSolver->_pParti[p]->m_ForceAccumulator[i];
		}
	}
	printf("\n_WeightInverse ");
	for(int c = 0; c<Col;c++)
	{
		printf("\n");
		printf("%f",_WeightInverse[c]);
	}
	
	for(int r = 0; r<Row;r++)
	{
		for(int c = 0; c<Col;c++)
		{
			_JT[c*Row+r]= this->_J[r*Row+c];
		}
	}

	for(int r = 0; r<Col;r++)
	{
		printf("\n_JT: ");
		for(int c = 0; c<Row;c++)
		{
			printf(",");
			printf("%f",_JT[r*Row+c]);
		}
	}

	//(J*W*_JT)x = -Jdot*xdot-J*W*forces;
	//Ax = b

	int steps=100;
	
	DATA* Jdotqdot = new DATA[Row*1];
	for(int r = 0; r<Row;r++)
	{
		Jdotqdot[r]=0;
		for(int c = 0; c<Col;c++)
		{
			Jdotqdot[r] += this->_JDot[r*Row+c]*this->_qDot[c];
		}
		Jdotqdot[r]*=-1.0;
	}
	
	

	printf("\n_qDot: ");
	for(int r = 0; r<Col;r++)
	{
		printf("%f, ",_qDot[r]);
	}

	printf("\nJdqd: ");
	for(int r = 0; r<Row;r++)
	{
		printf("%f, ",Jdotqdot[r]);
	}


	DATA* JWQ = new DATA[Row*1];
	for(int r = 0; r<Row;r++)
	{
		JWQ[r]=0;
		for(int c = 0; c<Col;c++)
		{
			JWQ[r] += 
				_J[r*Row+c]*_WeightInverse[c]*this->_QForces[c];
		}
		JWQ[r]*=-1.0;
	}
	printf("\_QForces: ");
	for(int c = 0; c<Col;c++)
	{
		printf("%f,",_QForces[c]);
	}
	printf("\nJdqd: ");
	for(int r = 0; r<Row;r++)
	{
		printf("%f",Jdotqdot[r]);
	}

	printf("\nJWQ: ");
	for(int r = 0; r<Row;r++)
	{
		printf("%f,",JWQ[r]);
	}
	
 	vecAddEqual(Row,Jdotqdot,JWQ);

	printf("\nRightSide b : ");
	for(int r = 0; r<Row;r++)
	{
		printf("%f,",Jdotqdot[r]);
	}
	for(int i=0;i<Row;i++)
		_Lamada[i]=0;
	//double err = ConjGrad(3, this,this->_Lamada, pb,1.0e-5, &steps);
	double err = ConjGrad(Row, this,this->_Lamada, Jdotqdot,1.0e-5, &steps);
	

	printf("\n_Lamada : ");
	for(int i=0;i<Row;i++)
		printf("%f,", _Lamada[i]);

	DATA* F = new DATA[Col];
	for(int r=0;r<Col;r++)
	{
		int p =(r-r%3)/3;
		DATA tmp =0;
		for(int c=0;c<Row;c++)
		{
			tmp+= this->_JT[r*Col+c]*_Lamada[c];
		}
		F[r] = tmp;
	}

	for(int r=0;r<this->pSolver->_numOfParti;r++)
	{
		this->pSolver->_pParti[r]->m_ForceAccumulator.x += F[r*3];
		this->pSolver->_pParti[r]->m_ForceAccumulator.y += F[r*3+1];
		this->pSolver->_pParti[r]->m_ForceAccumulator.z += F[r*3+2];
	}

	printf("\nForce : ");
	for(int i=0;i<Col;i++)
		printf("%f,", F[i]);

	printf("\nAccumulatorForce : ");
	for(int i=0;i<Col;i++)
		printf("%f,", this->pSolver->_pParti[(i-i%3)/3]->m_ForceAccumulator[i%3]);
 }

void ConstraintSolver::matVecMult(double x[], double b[])
{
	for(int r = 0; r<Row;r++)
	{
		for(int c = 0; c<Row;c++)
		{
			b[r] =0;
			for(int i = 0; i<Col;i++)
			{
				//(J*W*_JT)x = -Jdot*xdot-J*W*forces;
				b[r] +=  this->_J[r*Row+i]*_WeightInverse[i]*_JT[c*Row+r]*x[i];
			}
		}
	}
}

