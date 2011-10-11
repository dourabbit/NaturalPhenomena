#include "VerletIntegrater.hpp"

VerletIntegrater::VerletIntegrater(Cloth* cloth)
{
	this->_cloth=cloth;
}

VerletIntegrater::~VerletIntegrater(){}
void VerletIntegrater::Integrate(Vector3f acc, Particle* pParticle, float elaspedTime)
{
	
	Vector3f newPos= (2-_cloth->DRAG)* pParticle->m_Position
		-(1-_cloth->DRAG)*pParticle->prevPosition
		+acc*elaspedTime*elaspedTime;
	pParticle->prevPosition = pParticle->m_Position;
	pParticle->m_Position =newPos;

	pParticle->m_Velocity+= acc * elaspedTime;

}