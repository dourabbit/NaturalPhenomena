#ifndef _IMPLICITINTEGRATOR_HPP_
#define _IMPLICITINTEGRATOR_HPP_
#include <Vector/Vector.hpp>
#include <Particle/Particle.h>
#include <Integrators\Integrator.hpp>
#include <vector>
//#include <Cloth/Cloth.hpp>


class ImplicitIntegrator: public Integrator{

public:

	ImplicitIntegrator();
	
	virtual void Integrate(Solver solver, std::vector<Particle*> pVector, float elaspedTime,float* px,float* pDx);

	//Cloth* _cloth;
};
#endif