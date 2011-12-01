#include "EulerIntegrator.hpp"

EulerIntegrator::EulerIntegrator(Solver* solver):Integrator(solver)
{
	//this->_cloth=cloth;
	this->IntegratorNm = "EulerIntegrator";
	this->_pSolver = solver;
	
}

EulerIntegrator::~EulerIntegrator(){

	delete[] this->_phaseSpace;
	delete[] this->_phaseSpaceDot;
}

void EulerIntegrator:: Initialize()
{
	this->DIM = _pSolver->DIM;
	this->_phaseSpace = new DATA[_pSolver->DIM];
	this->_phaseSpaceDot = new DATA[_pSolver->DIM];

	memset(_phaseSpace, 0, _pSolver->DIM*sizeof(DATA));
	memset(_phaseSpaceDot, 0, _pSolver->DIM*sizeof(DATA));

};

void EulerIntegrator::Integrate(DATA elaspedTime)
{

	
	
	//int numOfParticle = this->_pSolver->_numOfParti;
	
	this->_pSolver->getState(this->_phaseSpace);
	this->_pSolver->getDerivative(this->_phaseSpaceDot);

	for(int i=0; i<this->DIM; i++)
	{
		
		this->_phaseSpace[i]+=elaspedTime*this->_phaseSpaceDot[i];
		
	}
	this->_pSolver->setState(this->_phaseSpace);
	
}