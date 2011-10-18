#ifndef _MIDPOINTINTEGRATOR_HPP_
#define _MIDPOINTINTEGRATOR_HPP_

#include <Integrators\Integrator.hpp>
#include <Integrators\Solver.hpp>
class MidPointIntegrator:public Integrator
{

public:

	MidPointIntegrator(Solver* solver);
	virtual ~MidPointIntegrator();
	//virtual void Integrate(Vector3f acc, Particle* pParticle, float elaspedTime);
	virtual void Integrate(DATA elaspedTime);
};
#endif
