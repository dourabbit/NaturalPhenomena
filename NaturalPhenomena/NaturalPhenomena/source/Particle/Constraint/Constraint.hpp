
#ifndef _CONSTRAINT_HPP_
#define _CONSTRAINT_HPP_
//#include <Cloth/Cloth.hpp>
class Constraint{

public:

	Constraint();
	virtual ~Constraint();
	virtual void draw() = 0;
	//virtual void apply_force() = 0;
	//Cloth* _cloth;
};

#endif
