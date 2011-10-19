
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
	//delete[] _Lamada;
}


void ConstraintSolver::Initialize()
{
	Row = this->pSolver->_pConstraints.size();
	Col = this->pSolver->_numOfParti * 3;
	
	this->_J = new DATA[Row*Col];
	this->_WeightInverse = new DATA[Col];
	this->_JT = new DATA[Col*Row];
	this->_JDot = new DATA[Row*Col];
	this->_qDot = new DATA[Col*1];
	this->_QForces = new DATA[Col*1];
	

	this->_isInitialized = true;

}
void ConstraintSolver::Solve(DATA* dst)
{
	if(!_isInitialized)
		this->Initialize();

	DATA* _Lamada = new DATA[Row*1];
	memset(_Lamada, 0, Row*sizeof(double));

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
				//printf("?%f==%f,",pParti->m_Velocity[k],_JDot[row*(Col)+col]);
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
	for(int r = 0; r<Row;r++)
	{
		printf("\n_J");
		for(int c = 0; c<Col;c++)
		{
			printf(",");
			printf("%f",_J[r*Col+c]);
		}
		printf("\n_JDot");
		for(int c = 0; c<Col;c++)
		{
			printf(",");
			printf("%f",_JDot[r*Col+c]);
		}
		printf("\n_qDot");
		for(int c = 0; c<Col;c++)
		{
			printf(",");
			printf("%f",_qDot[c]);
		}
		printf("\n_QForces");
		for(int c = 0; c<Col;c++)
		{
			printf(",");
			printf("%f",_QForces[c]);
		}
	}
	
	for(int r = 0; r<Row;r++)
	{
		for(int c = 0; c<Col;c++)
		{
			_JT[c*Row+r]= this->_J[r*Row+c];
		}
	}

	

	//(J*W*_JT)x = -Jdot*xdot-J*W*forces;
	//Ax = b

	int steps=100;
	
	DATA* Jdotqdot = new DATA[Row*1];
	for(int r = 0; r<Row;r++)
	{
		Jdotqdot[r]=0.0;
		for(int c = 0; c<Col;c++)
		{
			Jdotqdot[r] += this->_JDot[r*Col+c]*this->_qDot[c];
			printf("\nJdotqdot??%f*%f",_JDot[r*Col+c],_qDot[c]);
		}
		printf("====%f",Jdotqdot[r]);
	}
	
	

	


	DATA* JWQ = new DATA[Row*1];
	for(int r = 0; r<Row;r++)
	{
		JWQ[r]=0.0;
		for(int c = 0; c<Col;c++)
		{
			JWQ[r] += 
				_J[r*Col+c]*_WeightInverse[c]*this->_QForces[c];
			printf("\n\nJWQ??%f*%f",_J[r*Col+c],_QForces[c]);
		}
		printf("====%f",JWQ[r]);
	}

	for(int r = 0; r<Row;r++)
	{
		printf("\n\nJdotqdot %f,",Jdotqdot[r]);
	}

	vecTimesScalar(Row,Jdotqdot,-1.0);
	vecTimesScalar(Row,JWQ,-1.0);
 	vecAddEqual(Row,Jdotqdot,JWQ);

	
	
	for(int r = 0; r<Row;r++)
	{
		printf("\n\nJWQ %f,",JWQ[r]);
	}

	for(int r = 0; r<Row;r++)
	{
		printf("\n\n B is :: %f,",Jdotqdot[r]);
	}

	for(int i=0;i<Row;i++)
		_Lamada[i]=0;
	printf("\nBeforeConjGrad: %f,\n",_Lamada[0]);

	double err = ConjGrad(Row, this,_Lamada, Jdotqdot,1.0e-5, &steps);
	
	printf("\n\n\n\n\n\nConjGrad: %f,",_Lamada[0]);

	double tmp = 0.0;
	for(int c = 0; c<Col;c++){
		tmp +=  _J[c]*_WeightInverse[c]*_JT[c];
		//printf("\n>_<? %f,%f,%f",_J[c],_WeightInverse[c],_JT[c]);
	}
	_Lamada[0] = Jdotqdot[0]/tmp;
	
	printf(", (: %f),\n",_Lamada[0]);

	DATA* F = new DATA[Col];
	for(int r=0;r<Row;r++)
	{
		int p =(r-r%3)/3;
		
		for(int c=0;c<Col;c++)
		{
			F[c] = this->_JT[r*Col+c]*_Lamada[r];
		}
		
		
	}

	printf("\n\nJT");
	for(int i=0;i<Col;i++)
		printf(",%f",_JT[i]);

	printf("\n\nForce");
	for(int i=0;i<Col;i++)
		printf(",%f",F[i]);


	printf("\n\nX is !!: %f",_Lamada[0]);
	for(int r=0;r<this->pSolver->_numOfParti;r++)
	{
		this->pSolver->_pParti[r]->m_ForceAccumulator.x += F[r*3];
		this->pSolver->_pParti[r]->m_ForceAccumulator.y += F[r*3+1];
		this->pSolver->_pParti[r]->m_ForceAccumulator.z += F[r*3+2];
	}

	
	for(int ii=0; ii<this->pSolver->_numOfParti;ii++)
	{
		int particleIndex = ii;

		*(dst+6*ii)   = this->pSolver->_pParti[particleIndex]->m_Velocity.x;
		*(dst+1+6*ii) = this->pSolver->_pParti[particleIndex]->m_Velocity.y;
		*(dst+2+6*ii) = this->pSolver->_pParti[particleIndex]->m_Velocity.z;

		Vector<DATA,3> acc = (this->pSolver->_pParti[particleIndex]->m_ForceAccumulator/
							  this->pSolver->_pParti[ii]->m_Mass);
		*(dst+3+6*ii) = acc.x;
		*(dst+4+6*ii) = acc.y;
		*(dst+5+6*ii) = acc.z;
	}

	//delete[] _Lamada;

	

	/*printf("\n\n==================================================================: ");
		printf("\n\nPos:%f,",this->pSolver->_pParti[0]->m_Position.x,",");
		printf("%f,",this->pSolver->_pParti[0]->m_Position.y,",");
		printf("%f,",this->pSolver->_pParti[0]->m_Position.z),",";
		printf("\n");
		printf("Vel:%f,",this->pSolver->_pParti[0]->m_Velocity.x,",");
		printf("%f,",this->pSolver->_pParti[0]->m_Velocity.y,",");
		printf("%f,",this->pSolver->_pParti[0]->m_Velocity.z,",");
		Vector<DATA,3> acc = (this->pSolver->_pParti[0]->m_ForceAccumulator/
							  this->pSolver->_pParti[0]->m_Mass);
		printf("\n\nAcc:%f,",acc.x,",");
 		printf("%f,",acc.y,",");
		printf("%f,",acc.z,",");
	printf("\n\n==================================================================: ");*/
   }

void ConstraintSolver::matVecMult( double x[], double b[])
{
	//for(int r = 0; r<Row;r++)
	//{
	//	b[r] = 0.0;
	//	for(int c = 0; c<Col;c++)
	//	{
	//		//printf("\n???????, %f ", _J[r*Col+i]*_WeightInverse[i]*_JT[r*Col+c]*x[r]);
	//		//for(int i = 0; i<Col;i++)
	//		//{
	//		//	//(J*W*JT)x = -Jdot*xdot-J*W*forces;
	//		//	b[r] +=  _J[r*Col+i]*_WeightInverse[i]*_JT[r*Col+c]*x[r];
	//		//}
	//		printf("\n??????????????????, %f ,%f", _J[r*Col+c],_JT[c*Row+r]);
	//		b[r] +=  _J[r*Col+c]*_WeightInverse[c]*_JT[c*Row+r]*x[r];
	//	}
	//}

	double tmp = 0.0;
	for(int c = 0; c<Col;c++){
		tmp +=  _J[c]*_WeightInverse[c]*_JT[c];
	}
	b[0] = tmp*x[0];
}

