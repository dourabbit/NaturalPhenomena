
#include <Integrators\ConstraintSolver.hpp>



ConstraintSolver::ConstraintSolver(Solver* solver):pSolver(solver){
	
	//Row = this->pSolver->_pConstraints.size();
	this->_isInitialized = false;
}

ConstraintSolver::~ConstraintSolver(){

	delete[] _Jacobi;
	delete[] _WeightInverse;
	
	delete[] _JacobiDot;
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
	this->_Jacobi = new DATA[Row*Col];
	this->_WeightInverse = new DATA[Col*Col];
	this->JT = new DATA[Col*Row];
	this->_JacobiDot = new DATA[Row*Col];
	this->_qDot = new DATA[Col*1];
	this->_QForces = new DATA[Col*1];
	this->_Lamada = new DATA[Col*1];
	
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
	for(int i=0;i<this->pSolver->_pConstraints.size();i++)
	{
		
		for(int j=0;j<this->pSolver->_numOfParti;j++)
		{
			for(int k=0;k<3;k++)
			{
				Particle* pParti = this->pSolver->_pConstraints[i]->m_p1;

				this->_Jacobi[i*(pSolver->_numOfParti*3)+(j*3)+k] = pParti->m_Position[k];
			}

			//Fill Jacobian Matrix
			if(pSolver->_pConstraints[i]->m_p1->ParticleID==j)
			{
				pSolver->_pConstraints[i]->partialDx(&_Jacobi[i*(pSolver->_numOfParti*3)+(j*3)]);
				pSolver->_pConstraints[i]->partialDDx(&_JacobiDot[i*(pSolver->_numOfParti*3)+(j*3)]);
			}
			else
				for(int k=0;k<3;k++)
				{
					_Jacobi[i*(pSolver->_numOfParti*3)+(j*3)+k] = 0;
					_JacobiDot[i*(pSolver->_numOfParti*3)+(j*3)+k] = 0;
				}
		}
	}
	for(int r = 0; r<Row;r++)
	{
		printf("\n");
		for(int c = 0; c<Col;c++)
		{
			printf(",");
			printf("%f",_Jacobi[r*Row+c]);
		}
	}
	//Fill _WeightInverse Matrix &
	for(int r=0;r<Col;r++)
	{
		int p =(r-r%3)/3;
		for(int c=0;c<Col;c++)
		{
			this->_WeightInverse[r*Col+c]=
			r==c?1.0/(pSolver->_pParti[p]->m_Mass):0;
		}
		for(int i=0;i<3;i++)
		{
			_qDot[p*3+i]= pSolver->_pParti[p]->m_Velocity[i];
			_QForces[p*3+i] =  pSolver->_pParti[p]->m_ForceAccumulator[i];
		}
	}

	
	for(int r = 0; r<Row;r++)
	{
		for(int c = 0; c<Col;c++)
		{
			JT[c*Row+r]= this->_Jacobi[r*Row+c];
		}
	}


	//(J*W*JT)x = -Jdot*xdot-J*W*forces;
	//Ax = b

	//LeftHandSide
	/*MatrixP J_m = MatrixP(Row,Col, this->_Jacobi);
	MatrixP W_m = MatrixP(Col,Col, this->_WeightInverse);
	MatrixP JT_m = J_m.Transpose(J_m);
	MatrixP tmp =J_m.MultiplyP(J_m,W_m);
	this->_leftHandSide_m = J_m.MultiplyP(tmp,JT_m);*/
	
	//RightHandSide
	/*MatrixP JDot_m = MatrixP(Row,Col, this->_JacobiDot);
	MatrixP qDot_m = MatrixP(Col,1, this->_qDot);
	
	MatrixP JDotqDot_m = MatrixP(Row,1);
	MatrixP QForces_m = MatrixP(Col,1,this->_QForces);
	JDotqDot_m = JDotqDot_m.MultiplyP(JDotqDot_m.MultiplyP(JDot_m,qDot_m),-1.0);

	MatrixP JWQ_m = MatrixP(Row,1);
	JWQ_m = JWQ_m.MultiplyP(JWQ_m.MultiplyP(J_m,W_m),QForces_m);
	JWQ_m = JWQ_m.MultiplyP(JWQ_m, -1.0);

	this->_rightHandSide_m = JWQ_m.AddP(JDotqDot_m,JWQ_m);*/

	///*DATA a[4] = {0,0,1,1};
	//DATA b[4] = {0,0.1,20,1};
	//DATA m[4] = {0,1,1,0};
	//MatrixP e = MatrixP(2,2,&a[0]);
	//MatrixP c = MatrixP(2,2,&b[0]); 
	//MatrixP d = MatrixP(2,2,&m[0]); 

	//MatrixP dd = c.MultiplyP(c.MultiplyP(e,c),d);*/

	


	//Multiply _Jacobi and Weight
	//Multiply(*_Jacobi,Row,Col,*Weight,Col,Col,tmp);
	//Multiply _Jacobi and Weight
	//Multiply(*tmp,Row,Col,*_Jacobi_T,Col,Row,tmp);
	int steps=100;
	/*DATA* pb = new DATA[Row];
	for(int i=0; i<Row; i++)
		pb[i] = this->_rightHandSide_m.Data[i];*/

	//DATA* pb = this->_rightHandSide_m.ToArray();
	
	//(J*W*JT)x = -Jdot*xdot-J*W*forces;
	DATA* Jdotqdot = new DATA[Row*1];
	for(int r = 0; r<Row;r++)
	{
		Jdotqdot[r]=0;
		for(int c = 0; c<Col;c++)
		{
			Jdotqdot[r] += this->_JacobiDot[r*Row+c]*this->_qDot[c];
		}
		Jdotqdot[r]*=-1.0;
	}
	DATA* JWQ = new DATA[Row*1];
	for(int r = 0; r<Row;r++)
	{
		JWQ[r]=0;
		for(int c = 0; c<Col;c++)
		{
			DATA w = r==c? this->_WeightInverse[r*Row+c]:1;
			Jdotqdot[r] += this->_Jacobi[r*Row+c]*w*this->_QForces[c];
		}
		JWQ[r]*=-1.0;
	}
	vecDiffEqual(Row,Jdotqdot,JWQ);
	//double err = ConjGrad(3, this,this->_Lamada, pb,1.0e-5, &steps);
	double err = ConjGrad(3, this,this->_Lamada, Jdotqdot,1.0e-5, &steps);
	


	for(int i=0;i<3;i++)
		printf("%d", _Lamada[i]);

	DATA* F = new DATA[Col];
	for(int r=0;r<Col;r++)
	{
		int p =(r-r%3)/3;
		DATA tmp =0;
		for(int c=0;c<Row;c++)
		{
			tmp+= this->JT[r*Col+c]*_Lamada[c];
		}
		F[r] = tmp;
			
	}


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
				//b[r] += this->_leftHandSide_m(r,c)*x[c];
				DATA w = r==c? this->_WeightInverse[r*Row+c]:1;
				b[r] +=  this->_Jacobi[r*Row+i]*w*JT[c*Row+r];
			}
		}
	}
}

