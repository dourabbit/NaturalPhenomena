#pragma once

#include <Particle\Particle.h>
#include <Particle\Constraint\Constraint.hpp>
class CircularWireConstraint :public Constraint {
 public:
	CircularWireConstraint(Particle *p, const Vector<DATA,3> & center,
	  const double radius);

	virtual void draw();
	virtual void partialDx(DATA &x);
	virtual void partialDDx(DATA &x);
 private:

  //Particle * const m_p;
  Vector<DATA,3> const m_center;
  double const m_radius;
};
