#include "MidPointIntegrator.hpp"

MidPointIntegrator::MidPointIntegrator(Solver* solver):Integrator(solver)
{
	//this->_cloth=cloth;
	this->IntegratorNm = "MidPointIntegrator";
	this->_pSolver = solver;
}

MidPointIntegrator::~MidPointIntegrator(){}

void MidPointIntegrator:: Initialize()
{
	this->DIM = _pSolver->DIM;

	this->_phaseSpace = new DATA[_pSolver->DIM];
	this->_phaseSpaceDot = new DATA[_pSolver->DIM];
	this->_midPhaseSpace = new DATA[_pSolver->DIM];
	this->_midPhaseSpaceDot = new DATA[_pSolver->DIM];

	memset(_phaseSpace, 0, _pSolver->DIM*sizeof(DATA));
	memset(_phaseSpaceDot, 0, _pSolver->DIM*sizeof(DATA));
	memset(_midPhaseSpace, 0, _pSolver->DIM*sizeof(DATA));
	memset(_midPhaseSpaceDot, 0, _pSolver->DIM*sizeof(DATA));

};

void MidPointIntegrator::Integrate(DATA elaspedTime)
{


	this->_pSolver->getState(this->_phaseSpace);
	this->_pSolver->getDerivative(this->_phaseSpaceDot);

	for(int i=0; i<this->DIM; i++)
	{
		this->_midPhaseSpace[i]=this->_phaseSpace[i]+elaspedTime*0.5*this->_phaseSpaceDot[i];
	}
	this->_pSolver->setState(this->_midPhaseSpace);

	
	this->_pSolver->getDerivative(this->_midPhaseSpaceDot);
	for(int i=0; i<this->DIM; i++)
	{
		this->_phaseSpace[i]+=elaspedTime*this->_midPhaseSpaceDot[i];
	}

	this->_pSolver->setState(this->_phaseSpace);

	//for(int i=0; i<DIM; i++)
	//{
	//	printf("\n== %f+ ==\t %f",this->_phaseSpaceDot[i],
	//	this->_phaseSpace[i]);
	//}
	////printf("\n=======================================\n\n");


}