#ifndef _EULERINTEGRATER_HPP_
#define _EULERINTEGRATER_HPP_

#include "Integrater.hpp"
class EulerIntegrater:public Integrater
{

public:

	EulerIntegrater(Cloth* cloth);
	virtual ~EulerIntegrater();
	virtual void Integrate(Vector3f acc, Particle* pParticle, float elaspedTime);
};
#endif
