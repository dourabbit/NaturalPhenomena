#pragma once

#include <Particle\Particle.h>
#include <Particle\Force\Force.hpp>
#include <Vector\Vector.hpp>
class Gravity:public Force {
 public:
	  Gravity(Particle *p1);

	virtual void draw();
	virtual void apply_force();

	 private:
	  Particle * const m_p1;   // particle 1
};
