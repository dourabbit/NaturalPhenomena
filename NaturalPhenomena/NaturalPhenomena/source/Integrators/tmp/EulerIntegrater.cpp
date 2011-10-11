#include "EulerIntegrater.hpp"

EulerIntegrater::EulerIntegrater(Cloth* cloth)
{
	this->_cloth=cloth;
}

EulerIntegrater::~EulerIntegrater(){}
void EulerIntegrater::Integrate(Vector3f acc, Particle* pParticle, float elaspedTime)
{
	acc-=_cloth->DRAG*pParticle->m_Velocity;
	pParticle->m_Position+= pParticle->m_Velocity*elaspedTime;
	pParticle->m_Velocity+= acc * elaspedTime;
}