#include "PointConstrain.hpp"

PointConstrain::PointConstrain(Particle *p1) : 
_pConstrainedParticle(p1)
{
	_orginalPos= p1->m_Position;

}

void PointConstrain::draw()
{
}

void PointConstrain::apply_Constrain()
{
	//_pConstrainedParticle->m_Position = _orginalPos;
	_pConstrainedParticle->reset();
}

