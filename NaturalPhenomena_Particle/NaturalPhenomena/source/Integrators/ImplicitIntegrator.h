#ifndef _IMPLICITINTEGRATOR_HPP_
#define _IMPLICITINTEGRATOR_HPP_
//#include <Vector/Vector.hpp>
//#include <Particle/Particle.h>
//#include <Integrators\Integrator.hpp>
//#include <vector>
////#include <Cloth/Cloth.hpp>
//
//
//class ImplicitIntegrator: public Integrator{
//
//public:
//
//	ImplicitIntegrator(Solver* solver);
//	
//	virtual void Integrate(std::vector<Particle*> pVector, float elaspedTime,float* px,float* pDx);
//
//	//Cloth* _cloth;
//};
//#endif
#include <Vector/Vector.hpp>
#include <Particle/Particle.h>
#include <Integrators\Integrator.hpp>
#include <vector>
#include <Integrators\linearSolver.h>
class Solver;
class ImplicitIntegrator: public Integrator, public implicitMatrix{

public:

	ImplicitIntegrator(Solver* solver);
	
	virtual void Integrate(DATA elaspedTime);
	virtual void matVecMult(double x[], double r[]);
	//Cloth* _cloth;
private:
	Solver* _pSolver;
};
#endif