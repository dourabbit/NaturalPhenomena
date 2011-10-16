#include "EulerIntegrator.hpp"

EulerIntegrator::EulerIntegrator(Solver* solver):Integrator(solver)
{
	//this->_cloth=cloth;
	this->IntegratorNm = "EulerIntegrator";
}

EulerIntegrator::~EulerIntegrator(){}
void EulerIntegrator::Integrate(std::vector<Particle*> pParticles, 
							DATA elaspedTime)
{
	
	//acc-=_cloth->DRAG*pParticle->m_Velocity;
	for(int i=0;i<pParticles.size();i++)
	{
		Particle* pParticle = pParticles[i];
		Vector<DATA,3> acc = pParticle->m_ForceAccumulator/pParticle->m_Mass;
		pParticle->m_Position+= pParticle->m_Velocity*elaspedTime;
		pParticle->m_Velocity+=  acc* elaspedTime;
	}
}