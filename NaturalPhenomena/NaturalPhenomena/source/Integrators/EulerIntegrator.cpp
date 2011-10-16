#include "EulerIntegrator.hpp"

EulerIntegrator::EulerIntegrator(Solver* solver):Integrator(solver)
{
	//this->_cloth=cloth;
}

EulerIntegrator::~EulerIntegrator(){}
void EulerIntegrator::Integrate(std::vector<Particle*> pParticles, 
							float elaspedTime,
							float* px,float* pDx)
{
	//acc-=_cloth->DRAG*pParticle->m_Velocity;
	for(int i=0;i<pParticles.size();i++)
	{
		Particle* pParticle = pParticles[i];
		Vector<float,3> acc = pParticle->m_ForceAccumulator/pParticle->m_Mass;
		pParticle->m_Position+= pParticle->m_Velocity*elaspedTime;
		pParticle->m_Velocity+=  acc* elaspedTime;
	}
}