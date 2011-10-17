
#include <Integrators\ConstraintSolver.hpp>

ConstraintSolver::ConstraintSolver(Solver* solver):pSolver(solver){
	
	//Row = this->pSolver->_pConstraints.size();

}

ConstraintSolver::~ConstraintSolver(){}


void ConstraintSolver::matVecMult(double x[], double r[])
{




}

void ConstraintSolver::Solve()
{
	int Row = this->pSolver->_pConstraints.size();
	int Col = this->pSolver->_numOfParti * 3;

	Jacobi = new DATA[Row*Col];
	Weight = new DATA[Col*Col];
	Jacobi_T = new DATA[Col*Row];

	MatrixP Jm = MatrixP(Row,Col, Jacobi);
	MatrixP Wm = MatrixP(Col,Col, Weight);
	MatrixP JmT = MatrixP(Col,Row, Jacobi_T);

	MatrixP result = Jm.MultiplyP(Jm.MultiplyP(Jm,Wm),JmT);
	
	delete[] Jacobi;
	delete[] Weight;
	delete[] Jacobi_T;
	 
	///*DATA a[4] = {0,0,1,1};
	//DATA b[4] = {0,0.1,20,1};
	//DATA m[4] = {0,1,1,0};
	//MatrixP e = MatrixP(2,2,&a[0]);
	//MatrixP c = MatrixP(2,2,&b[0]); 
	//MatrixP d = MatrixP(2,2,&m[0]); 

	//MatrixP dd = c.MultiplyP(c.MultiplyP(e,c),d);*/

	


	//Multiply Jacobi and Weight
	//Multiply(*Jacobi,Row,Col,*Weight,Col,Col,tmp);
	//Multiply Jacobi and Weight
	//Multiply(*tmp,Row,Col,*Jacobi_T,Col,Row,tmp);

	
	
	
}

void ConstraintSolver::matVecMult(double x[], double b[])
{
	

}

