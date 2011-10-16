
#ifndef _FORCE_HPP_
#define _FORCE_HPP_
//#include <Cloth/Cloth.hpp>
#include <Vector\Vector.hpp>
#include <common.h>

class Particle;
class Force{

public:

	Force();
	virtual ~Force();
	virtual void draw() = 0;
	virtual void apply_force() = 0;
	virtual void partialDx(DATA &x)=0 ;
	virtual void partialDDx(DATA &x)=0 ;

	virtual Vector<DATA,3> guessForce(Vector<DATA,3> &position, Particle &partilce)=0;
	//Cloth* _cloth;
};

#endif
