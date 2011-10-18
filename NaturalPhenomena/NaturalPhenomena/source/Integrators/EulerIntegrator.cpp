#include "EulerIntegrator.hpp"

EulerIntegrator::EulerIntegrator(Solver* solver):Integrator(solver)
{
	//this->_cloth=cloth;
	this->IntegratorNm = "EulerIntegrator";
	this->_pSolver = solver;
}

EulerIntegrator::~EulerIntegrator(){}
void EulerIntegrator::Integrate(DATA elaspedTime)
{
	
	int numOfParticle = this->_pSolver->_numOfParti;
	
	for(int i=0; i<numOfParticle*6; i++)
	{
		
		this->_pSolver->PhaseSpace[i]+=elaspedTime*this->_pSolver->PhaseSpaceDot[i];
		
	}
}