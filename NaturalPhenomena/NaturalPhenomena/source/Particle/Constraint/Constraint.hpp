
#ifndef _CONSTRAINT_HPP_
#define _CONSTRAINT_HPP_
//#include <Cloth/Cloth.hpp>
#include <Vector\Vector.hpp>
#include <Integrators\linearSolver.h>
class Constraint{

public:

	Constraint();
	virtual ~Constraint();
	virtual void draw() = 0;
	//virtual void apply_force() = 0;
	//Cloth* _cloth;
	
	virtual void partialDx(float &x)=0;
	virtual void partialDDx(float &x)=0;
};

#endif
