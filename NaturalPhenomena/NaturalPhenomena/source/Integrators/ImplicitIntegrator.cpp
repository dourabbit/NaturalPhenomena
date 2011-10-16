

//#include <Integrators\ImplicitIntegrator.h>
//#include <Integrators\common.h>
#include <Integrators\Solver.hpp>
#include <Particle\Force\Force.hpp>
#include <Integrators\linearSolver.h>
#include <Integrators\ImplicitIntegrator.h>
#include <Integrators\Integrator.hpp>
ImplicitIntegrator::ImplicitIntegrator(Solver* pSolver):
					Integrator(pSolver)
{
	//this->_pSolver = pSolver;


}

void ImplicitIntegrator::Integrate(std::vector<Particle*> 
			pVector,float elaspedTime,float* px,float* pDx)
{




}
void ImplicitIntegrator::matVecMult(double x[], double r[])
{
	std::vector<Force*>  pForces = this->_pSolver->_pForces;

	
	for(int i=0;i<pForces.size();i++)
	{

	
	}
}

