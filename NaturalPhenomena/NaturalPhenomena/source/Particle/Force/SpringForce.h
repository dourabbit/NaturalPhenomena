#pragma once

#include <Particle\Particle.h>
#include <Particle\Force\Force.hpp>
#include <Vector\Vector.hpp>
class SpringForce:public Force {
 public:
	  SpringForce(Particle *p1, Particle * p2, double dist, double ks, double kd);

	virtual void draw();
	virtual void apply_force();
	virtual Vector<float,3> partialDx() ;
	virtual Vector<float,3> partialDDx() ;
 private:
	  
	  Vector<float,3> _direction;
	  Particle * const m_p1;   // particle 1
	  Particle * const m_p2;   // particle 2 
	  double const m_dist;     // rest length
	  double m_ks, m_kd; // spring strength constants
};
