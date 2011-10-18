#ifndef _RUNGEKUTTA_HPP_
#define _RUNGEKUTTA_HPP_

#include <Integrators\Integrator.hpp>
#include <Integrators\Solver.hpp>
class RungeKutta:public Integrator
{

public:

	RungeKutta(Solver* solver);
	virtual ~RungeKutta();
	//virtual void Integrate(Vector3f acc, Particle* pParticle, float elaspedTime);
	virtual void Integrate(DATA elaspedTime);
};
#endif
