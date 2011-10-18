#ifndef _EULERINTEGRATER_HPP_
#define _EULERINTEGRATER_HPP_

#include <Integrators\Integrator.hpp>
#include <Integrators\Solver.hpp>
class EulerIntegrator:public Integrator
{

public:

	EulerIntegrator(Solver* solver);
	virtual ~EulerIntegrator();
	//virtual void Integrate(Vector3f acc, Particle* pParticle, float elaspedTime);
	virtual void Integrate(DATA elaspedTime);

};
#endif
