
#ifndef _CONSTRAINT_HPP_
#define _CONSTRAINT_HPP_
//#include <Cloth/Cloth.hpp>
#include <Vector\Vector.hpp>
#include <Integrators\linearSolver.h>
#include <common.h>
#include <Particle\Particle.h>
class Constraint{

public:

	Constraint(Particle* pParti);
	virtual ~Constraint();
	virtual void draw() = 0;
	//virtual void apply_force() = 0;
	//Cloth* _cloth;
	

	

	virtual void gradient(DATA* px)=0;
	virtual void gradientDot(DATA* px)=0;
	virtual DATA eval() = 0;
	virtual DATA evalDeriv() = 0;
	Particle * const m_p1;



};

#endif
