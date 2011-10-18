#include "RungeKutta.hpp"

RungeKutta::RungeKutta(Solver* solver):Integrator(solver)
{
	//this->_cloth=cloth;
	this->IntegratorNm = "RungeKutta";
	this->_pSolver = solver;
}

RungeKutta::~RungeKutta(){}

void RungeKutta:: Initialize()
{
	this->DIM = _pSolver->DIM;

	this->_phaseSpace = new DATA[_pSolver->DIM];
	this->_phaseSpaceDot = new DATA[_pSolver->DIM];
	
	this->_k1 = new DATA[_pSolver->DIM];
	this->_k2 = new DATA[_pSolver->DIM];
	this->_k3 = new DATA[_pSolver->DIM];
	this->_k4 = new DATA[_pSolver->DIM];

	this->_midPhaseSpace1 = new DATA[_pSolver->DIM];
	this->_midPhaseSpace2 = new DATA[_pSolver->DIM];
	this->_midPhaseSpace3 = new DATA[_pSolver->DIM];
	this->_midPhaseSpaceDot1 = new DATA[_pSolver->DIM];
	this->_midPhaseSpaceDot2 = new DATA[_pSolver->DIM];
	this->_midPhaseSpaceDot3 = new DATA[_pSolver->DIM];


	memset(_phaseSpace, 0, _pSolver->DIM*sizeof(DATA));
	memset(_phaseSpaceDot, 0, _pSolver->DIM*sizeof(DATA));
	memset(_k1, 0, _pSolver->DIM*sizeof(DATA));
	memset(_k2, 0, _pSolver->DIM*sizeof(DATA));
	memset(_k3, 0, _pSolver->DIM*sizeof(DATA));
	memset(_k4, 0, _pSolver->DIM*sizeof(DATA));
	memset(_midPhaseSpace1, 0, _pSolver->DIM*sizeof(DATA));
	memset(_midPhaseSpace2, 0, _pSolver->DIM*sizeof(DATA));
	memset(_midPhaseSpace3, 0, _pSolver->DIM*sizeof(DATA));
	memset(_midPhaseSpaceDot1, 0, _pSolver->DIM*sizeof(DATA));
	memset(_midPhaseSpaceDot2, 0, _pSolver->DIM*sizeof(DATA));
	memset(_midPhaseSpaceDot3, 0, _pSolver->DIM*sizeof(DATA));
};

void RungeKutta::Integrate(DATA elaspedTime)
{

	this->_pSolver->getState(this->_phaseSpace);
	this->_pSolver->getDerivative(this->_phaseSpaceDot);

	for(int i=0; i<this->DIM; i++)
	{
		this->_k1[i] = elaspedTime*_phaseSpaceDot[i];
		this->_midPhaseSpace1[i]=this->_phaseSpace[i]+elaspedTime*0.5*_k1[i];
	}
	this->_pSolver->setState(this->_midPhaseSpace1);

	this->_pSolver->getDerivative(this->_midPhaseSpaceDot1);

	for(int i=0; i<this->DIM; i++)
	{
		this->_k2[i] = elaspedTime*_midPhaseSpaceDot1[i];
		this->_midPhaseSpace2[i]=this->_phaseSpace[i]+elaspedTime*0.5*_k2[i];
	}
	this->_pSolver->setState(this->_midPhaseSpace2);

	this->_pSolver->getDerivative(this->_midPhaseSpaceDot2);

	for(int i=0; i<this->DIM; i++)
	{
		this->_k3[i] = elaspedTime*_midPhaseSpaceDot2[i];
		this->_midPhaseSpace3[i]=this->_phaseSpace[i]+elaspedTime*_k3[i];
	}
	this->_pSolver->setState(this->_midPhaseSpace3);


	this->_pSolver->getDerivative(this->_midPhaseSpaceDot3);

	for(int i=0; i<this->DIM; i++)
	{
		this->_k4[i] = elaspedTime*_midPhaseSpaceDot3[i];
		this->_phaseSpace[i]+= 1.0f/6.0f*_k1[i] + 1.0f/3.0f*_k2[i] + 1.0f/3.0f*_k3[i] + 1.0f/6.0f*_k4[i];
	}
	this->_pSolver->setState(this->_phaseSpace);

	
}