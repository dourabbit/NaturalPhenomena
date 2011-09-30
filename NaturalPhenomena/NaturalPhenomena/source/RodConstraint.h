#pragma once

#include "Particle.h"

class RodConstraint {
 public:
  RodConstraint(Particle *p1, Particle * p2, double dist);

  void draw();

 private:

  Particle * const m_p1;
  Particle * const m_p2;
  double const m_dist;
};
