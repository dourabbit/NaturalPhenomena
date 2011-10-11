#ifndef _CONSTRAIN_HPP_
#define _CONSTRAIN_HPP_
//
//#include <Particle/Particle.hpp>
//#include <Particle/Force.hpp>

class Constrain
{

public:
	Constrain();
	virtual ~Constrain();
	virtual void draw()=0;
	virtual void apply_Constrain()=0;

//private:
//	Vector3f _orginalPos; //particle1
};

#endif