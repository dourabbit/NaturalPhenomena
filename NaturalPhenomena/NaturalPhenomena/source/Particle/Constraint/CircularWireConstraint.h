#pragma once

#include <Particle\Particle.h>
#include <Particle\Constraint\Constraint.hpp>
class CircularWireConstraint :public Constraint {
 public:
	CircularWireConstraint(Particle *p, const Vector<DATA,3> & center,
	  const double radius);

	virtual void draw();
	virtual void gradient(DATA* const px);
	virtual void gradientDot(DATA* const px);
	virtual DATA eval();
	virtual DATA evalDeriv();
 private:

  //Particle * const m_p;
  Vector<DATA,3> const m_center;
  double const m_radius;
};
