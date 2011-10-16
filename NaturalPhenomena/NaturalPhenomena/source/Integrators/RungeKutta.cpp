#include "RungeKutta.hpp"

RungeKutta::RungeKutta(Solver* solver):Integrator(solver)
{
	//this->_cloth=cloth;
	this->IntegratorNm = "RungeKutta";
}

RungeKutta::~RungeKutta(){}
void RungeKutta::Integrate(std::vector<Particle*> pParticles, 
							DATA elaspedTime)
{

	

	//acc-=_cloth->DRAG*pParticle->m_Velocity;
	for(int i=0;i<pParticles.size();i++)
	{
		Particle* pParticle = pParticles[i];
		Vector<DATA,3> acc = pParticle->m_ForceAccumulator/pParticle->m_Mass;
		Vector<DATA,3> initialVelocity = pParticle->m_Velocity;
		Vector<DATA,3> vel = initialVelocity + acc*elaspedTime;
		//k1 = hF(x0,t0)
		Vector<DATA,3> k1= vel *elaspedTime;
		

		Vector<DATA,3> newForce = make_vector(0.0,0.0,0.0);
		int size = pParticle->Forces.size();
		for(int i=0; i<size; i++)
		{
			newForce+=
			pParticle->Forces[i]->guessForce(k1/2,*pParticle);
		}
		//k2 = hF(x0+(k1)/2,t0+h/2)
		acc = newForce/pParticle->m_Mass;
		vel = initialVelocity + acc*(elaspedTime/2.0);
		Vector<DATA,3> k2 = vel*elaspedTime;
		
		//k3 = hF(x0+(k2)/2,t0+h/2)
		newForce = make_vector(0.0,0.0,0.0);
		for(int i=0; i<size; i++)
		{
			newForce+=
			pParticle->Forces[i]->guessForce(k2/2,*pParticle);
		}
		acc = newForce/pParticle->m_Mass;
		vel = initialVelocity + acc*(elaspedTime/2.0);
		Vector<DATA,3> k3 = vel*elaspedTime;
		
		//k4 = hF(x0+k3,t0)
		newForce = make_vector(0.0,0.0,0.0);
		for(int i=0; i<size; i++)
		{
			newForce+=
			pParticle->Forces[i]->guessForce(k3,*pParticle);
		}
		acc = newForce/pParticle->m_Mass;
		vel = initialVelocity + acc*(elaspedTime);
		Vector<DATA,3> k4 = vel*elaspedTime;
		
		Vector<DATA,3> deltaX =((1.0f/6.0f)*k1+(1.0f/3.0f)*k2+(1.0f/3.0f)*k3+(1.0f/6.0f)*k4);
		pParticle->m_Position += deltaX;
		//pParticle->m_Position += pParticle->m_Velocity*elaspedTime;
		pParticle->m_Velocity = initialVelocity + deltaX/elaspedTime;
	}

	
}