#include "MidPointIntegrator.hpp"

MidPointIntegrator::MidPointIntegrator(Solver* solver):Integrator(solver)
{
	//this->_cloth=cloth;
	this->IntegratorNm = "MidPointIntegrator";
}

MidPointIntegrator::~MidPointIntegrator(){}
void MidPointIntegrator::Integrate(std::vector<Particle*> pParticles, 
							DATA elaspedTime)
{



	//acc-=_cloth->DRAG*pParticle->m_Velocity;
	for(int i=1;i<pParticles.size();i++)
	{
		Particle* pParticle = pParticles[i];
		Vector<DATA,3> acc = pParticle->m_ForceAccumulator/pParticle->m_Mass;
		pParticle->m_Velocity +=  acc* elaspedTime;
		Vector<DATA,3> deltaX= pParticle->m_Velocity*elaspedTime;
		

		Vector<DATA,3> newForce = make_vector(0.0,0.0,0.0);
		int size = pParticle->Forces.size();
		for(int i=0; i<size; i++)
		{
			newForce+=
			pParticle->Forces[i]->guessForce(deltaX/2,*pParticle);
		}

		Vector<DATA,3> newVelocity = pParticle->m_Velocity + newForce/pParticle->m_Mass* (elaspedTime/2.0f);
		pParticle->m_Position += newVelocity*elaspedTime;
		//pParticle->m_Position += pParticle->m_Velocity*elaspedTime;

	}
}