#ifndef _INTEGRATOR_HPP_
#define _INTEGRATOR_HPP_
#include <Particle/Particle.h>
#include <vector>

class Integrator{

public:

	Integrator();
	virtual ~Integrator();
	virtual void Integrate( std::vector<Particle*> pVector, float elaspedTime,float* px,float* pDx) = 0;

	//Cloth* _cloth;
};
#endif
