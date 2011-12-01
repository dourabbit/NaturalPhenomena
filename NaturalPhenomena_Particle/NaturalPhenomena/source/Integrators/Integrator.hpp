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
	virtual void Integrate(DATA elaspedTime) = 0;
	std::string IntegratorNm;  
	int DIM;
	virtual void Initialize()=0;
protected:
	Solver* _pSolver;
	DATA* _phaseSpace;
	DATA* _phaseSpaceDot;
	//Cloth* _cloth;
};
#endif
