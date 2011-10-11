
#ifndef _FORCE_HPP_
#define _FORCE_HPP_
//#include <Cloth/Cloth.hpp>
#include <Vector\Vector.hpp>
class Force{

public:

	Force();
	virtual ~Force();
	virtual void draw() = 0;
	virtual void apply_force() = 0;
	virtual Vector<float,3> Dx() = 0;
	virtual Vector<float,3> DDx() = 0;

	//Cloth* _cloth;
};

#endif
