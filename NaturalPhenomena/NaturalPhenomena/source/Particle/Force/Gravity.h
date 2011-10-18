#pragma once

#include <Particle\Particle.h>
#include <Particle\Force\Force.hpp>
#include <Vector\Vector.hpp>
class Gravity:public Force {
 public:
	  Gravity(Particle *p1);

	virtual void draw();
	virtual void apply_force();
	virtual void gradient(DATA &x) ;
	virtual void gradientDot(DATA &x) ;
	virtual Vector<DATA,3> guessForce(Vector<DATA,3> &position, Particle &partilce);


	 private:
	  Particle * const m_p1;   // particle 1
};
