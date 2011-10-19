	#pragma once

#include <Particle\Particle.h>
#include <Particle\Constraint\Constraint.hpp>
class RodConstraint :public Constraint{
 public:
  RodConstraint(Particle *p1, Particle * p2, double dist);

virtual void draw();
	virtual void gradient(DATA* const px);
	virtual void gradientDot(DATA* const px);

 private:

  //Particle * const m_p1;
  Particle * const m_p2;
  double const m_dist;
};
