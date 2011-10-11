#ifndef _VERLETINTEGRATOR_
#define _VERLETINTEGRATOR_
#include <Vector/Vector.hpp>
#include <Particle/tmp/Particle.hpp>
#include "Integrator.hpp"
class VerletIntegrater:public Integrater
{

public:

	VerletIntegrater(Cloth* cloth);
	virtual ~VerletIntegrater();
	virtual void Integrate(Vector3f acc, Particle* pParticle, float elaspedTime);

};
#endif
