#pragma once

#include <Particle\Particle.h>
#include <Particle\Force\Force.hpp>
#include <Vector\Vector.hpp>
#include <common.h>
class SpringForce:public Force {
 public:
	  SpringForce(Particle *p1, Particle * p2, double dist, 
		  double ks, double kd);

	virtual void draw();
	virtual void apply_force();
	virtual void gradient(DATA &x) ;
	virtual void gradientDot( DATA &x) ;

	virtual Vector<DATA,3> guessForce(Vector<DATA,3> &position, Particle &partilce);
 private:
	  
	  Vector<DATA,3> _direction;
	  Particle * const m_p1;   // particle 1
	  Particle * const m_p2;   // particle 2 
	  double const m_dist;     // rest length
	  double m_ks, m_kd; // spring strength constants
};
