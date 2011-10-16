#ifndef _INTEGRATOR_HPP_
#define _INTEGRATOR_HPP_
#include <Particle/Particle.h>
#include <vector>
//#include <Integrators\common.h>
#include <Integrators\Solver.hpp>
#include <common.h>
#include <string.h>
class Solver;
class Integrator{

public:

	Integrator(Solver* solver);
	virtual ~Integrator();
	virtual void Integrate(std::vector<Particle*> pParticles, 
							DATA elaspedTime) = 0;
	std::string IntegratorNm;  
private:
	Solver* _pSolver;
	//Cloth* _cloth;
};
#endif
