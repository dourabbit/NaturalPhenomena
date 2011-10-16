#pragma once

#include <Particle\Particle.h>
#include <Particle\Constraint\Constraint.hpp>
class CircularWireConstraint :public Constraint {
 public:
	CircularWireConstraint(Particle *p, const Vector<float,3> & center,
	  const double radius);

	virtual void draw();
	virtual void partialDx(float &x);
	virtual void partialDDx(float &x);
 private:

  Particle * const m_p;
  Vector<float,3> const m_center;
  double const m_radius;
};
