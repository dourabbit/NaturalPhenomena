
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
	
	memset(_J, 0, (Row*Col)*sizeof(DATA));
	memset(_WeightInverse, 0, (Col)*sizeof(DATA));
	memset(_JT, 0, (Row*Col)*sizeof(DATA));
	memset(_JDot, 0, (Row*Col)*sizeof(DATA));
	memset(_qDot, 0, (Col)*sizeof(DATA));
	memset(_QForces, 0, (Col)*sizeof(DATA));
	this->_isInitialized = true;

}
void ConstraintSolver::Solve(DATA* dst)
{
	if(!_isInitialized)
		this->Initialize();

	
	//printf("\n\n\n\n\n");
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
				//printf("??????????????????????????????????????????????????????????????%f,%f",_J[row*(Col)+col],pParti->m_Position[k]);
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
	
	/*printf("\n======_J:\n");
	for(int r = 0; r<Row;r++)
	{
		printf("\n\t");
		for(int c = 0; c<Col;c++)
		{
			printf(",");
			printf("%f",_J[r*Col+c]);
			
		}
		printf("\n\t");
	}
	printf("\n========_JDot:\n");
	for(int r = 0; r<Row;r++)
	{
		printf("\n\t");
		for(int c = 0; c<Col;c++)
		{
			printf(",");
			printf("%f",_JDot[r*Col+c]);
		}
		printf("\n\t");
	}
	printf("\n========__qDot:\n");
		printf("\n\t");
		for(int c = 0; c<Col;c++)
		{
			printf(",");
			printf("%f",_qDot[c]);
		}
		printf("\n\t");

	printf("\n========__QForces:\n");
		printf("\n\t");
		for(int c = 0; c<Col;c++)
		{
			printf(",");
			printf("%f",_QForces[c]);
		}
		printf("\n\t");
	
*/
	//Transpose J matrix
	for(int r = 0; r<Row;r++)
	{
		for(int c = 0; c<Col;c++)
		{
			_JT[c*Row+r]= this->_J[r*Col+c];
		}
	}

	
	/*printf("\n========JT:\n");
	for(int c = 0; c<Col;c++)
	{
		printf("\n\t");
		for(int r = 0; r<Row;r++)
		{
			printf(",");
			printf("%f",_JT[c*Row+r]);
		}
		printf("\n\t");
	}
	printf("\n========JT:\n");
	for(int i = 0; i<Col*Row;i++)
	{
		printf("%f,",_JT[i]);
	}
	printf("\n========JT:\n");*/
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
			//printf("\nJdotqdot??%f*%f",_JDot[r*Col+c],_qDot[c]);
		}
		//printf("====%f",Jdotqdot[r]);
	}
	
	

	


	DATA* JWQ = new DATA[Row*1];
	for(int r = 0; r<Row;r++)
	{
		JWQ[r]=0.0;
		for(int c = 0; c<Col;c++)
		{
			JWQ[r] += 
				_J[r*Col+c]*_WeightInverse[c]*this->_QForces[c];
			//printf("\n\nJWQ??%f*%f",_J[r*Col+c],_QForces[c]);
		}
		//printf("====%f",JWQ[r]);
	}

	/*for(int r = 0; r<Row;r++)
	{
		printf("\n\nJdotqdot %f,",Jdotqdot[r]);
	}*/

	vecTimesScalar(Row,Jdotqdot,-1.0);
	vecTimesScalar(Row,JWQ,-1.0);
 	vecAddEqual(Row,Jdotqdot,JWQ);

	DATA* C = new DATA[Row];
	DATA* CDot = new DATA[Row];
	memset(C, 0, Row*sizeof(double));
	memset(CDot, 0, Row*sizeof(double));
	for(int i=0 ; i< Row; i++)
		C[0] = this->pSolver->_pConstraints[i]->eval();

	for(int i=0 ; i< Row; i++)
		CDot[0] = this->pSolver->_pConstraints[i]->evalDeriv();


	vecTimesScalar(Row,C, -1*this->pSolver->Ks);
	vecTimesScalar(Row,CDot, -1*this->pSolver->Kd);
	vecAddEqual(Row,Jdotqdot,C);
	vecAddEqual(Row,Jdotqdot,CDot);
	
	printf("\n");
	for(int i=0 ; i< Row; i++)
		printf("C:%f,CDot:%f",C,CDot);
	printf("\n");
	for(int i=0 ; i< Row; i++)
		printf("B:%f",Jdotqdot);

	delete[] C;
	delete[] CDot;


	//vecTimesScalar(Row*Col);
	
	/*for(int r = 0; r<Row;r++)
	{
		printf("\n\nJWQ %f,",JWQ[r]);
	}*/

	/*for(int r = 0; r<Row;r++)
	{
		printf("\n\n B is :: %f,",Jdotqdot[r]);
	}
*/
	

	DATA* _Lamada = new DATA[Row*1];
	memset(_Lamada, 0, Row*sizeof(double));
	
	double err = ConjGrad(Row, this,_Lamada, Jdotqdot,1.0e-5, &steps);
	//err = ConjGrad(Row, this,x, Jdotqdot,1.0e-5, &steps);
	//printf("\nConjGrad: %f?%f,",_Lamada[0],x[0]);
	/*for(int i = 0; i<Row;i++)
		printf("\nConjGrad%d: %f,",i,_Lamada[i]);*/

	//double nSolveByHand = 0.0;
	//for(int c = 0; c<Col;c++){
	//	nSolveByHand +=  _J[c]*_WeightInverse[c]*_JT[c];
	//	//printf("\n>_<? %f,%f,%f",_J[c],_WeightInverse[c],_JT[c]);
	//}
	//nSolveByHand = Jdotqdot[0]/nSolveByHand;
	//printf("\nSolveByHand: %f,",nSolveByHand);


	DATA* F = new DATA[Col];
	for(int r=0;r<Col;r++)
	{
		//int p =(r-r%3)/3;
		//b[r] +=  _J[r*Col+c]*_WeightInverse[c]*_JT[c*Row+r]*x[r];
		F[r] =0;
		for(int c=0;c<Row;c++)
		{
			F[r] += this->_JT[r*Row+c]*_Lamada[c];
		}
		
		
	}



	/*printf("\n========Added Force:\n");
		printf("\n\t");
		for(int c = 0; c<Col;c++)
		{
			printf(",");
			printf("%f",F[c]);
		}
		printf("\n\t");


	printf("\n\nX is !!: %f",_Lamada[0]);*/
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

	
	/*for(int ii=0; ii<this->pSolver->_numOfParti;ii++)
	{
		printf("\n\n==========================================================Particle:%d:",ii);
		printf("\n\nPos:%f,",this->pSolver->_pParti[ii]->m_Position.x,",");
		printf("%f,",this->pSolver->_pParti[ii]->m_Position.y,",");
		printf("%f,",this->pSolver->_pParti[ii]->m_Position.z),",";
		printf("\n");
		printf("Vel:%f,",this->pSolver->_pParti[ii]->m_Velocity.x,",");
		printf("%f,",this->pSolver->_pParti[ii]->m_Velocity.y,",");
		printf("%f,",this->pSolver->_pParti[ii]->m_Velocity.z,",");
		Vector<DATA,3> acc = (this->pSolver->_pParti[ii]->m_ForceAccumulator/
							  this->pSolver->_pParti[ii]->m_Mass);
		printf("\n\nAcc:%f,",acc.x,",");
 		printf("%f,",acc.y,",");
		printf("%f,",acc.z,",");
		printf("\n\n==================================================================: ");
	}
*/
   }

void ConstraintSolver::matVecMult( double x[], double b[])
{
	for(int r = 0; r<Row;r++)
	{
		b[r] = 0.0;
		for(int c = 0; c<Col;c++)
		{
			//printf("\n???????, %f ", _J[r*Col+i]*_WeightInverse[i]*_JT[r*Col+c]*x[r]);
			//for(int i = 0; i<Col;i++)
			//{
			//	//(J*W*JT)x = -Jdot*xdot-J*W*forces;
			//	b[r] +=  _J[r*Col+i]*_WeightInverse[i]*_JT[r*Col+c]*x[r];
			//}
			//printf("\n??????????????????, %f ,%f", _J[r*Col+c],_JT[r*Col+c]);
			b[r] +=  _J[r*Col+c]*_WeightInverse[c]*_JT[c*Row+r]*x[r];
		}
	}

	/*double tmp = 0.0;
	for(int c = 0; c<Col;c++){
		tmp +=  _J[c]*_WeightInverse[c]*_JT[c];
	}
	b[0] = tmp*x[0];
	printf("??????????\nA is :\n?, %f ,%f", tmp);*/
}

