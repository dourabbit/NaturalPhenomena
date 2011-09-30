#pragma once

#include "Particle.h"

class CircularWireConstraint {
 public:
  CircularWireConstraint(Particle *p, const Vec2f & center, const double radius);

  void draw();

 private:

  Particle * const m_p;
  Vec2f const m_center;
  double const m_radius;
};
