#ifndef _INTEGRATOR_HPP_
#define _INTEGRATOR_HPP_
#include <Particle/Particle.h>
#include <vector>
//#include <Integrators\common.h>
#include <Integrators\Solver.hpp>

class Solver;
class Integrator{

public:

	Integrator(Solver* solver);
	virtual ~Integrator();
	virtual void Integrate(std::vector<Particle*> pParticles, 
							float elaspedTime,
							float* px,float* pDx) = 0;

private:
	Solver* _pSolver;
	//Cloth* _cloth;
};
#endif
