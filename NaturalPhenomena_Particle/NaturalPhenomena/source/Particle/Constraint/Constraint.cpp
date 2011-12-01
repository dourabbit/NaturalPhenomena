#include "Constraint.hpp"

Constraint::Constraint(Particle* pParti):m_p1(pParti){
  this->m_p1->pConstraint = this;
}

Constraint::~Constraint(){}
