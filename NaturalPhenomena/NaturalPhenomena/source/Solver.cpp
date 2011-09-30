#include "Particle.h"

#include <vector>

#define DAMP 0.98f
#define RAND (((rand()%2000)/1000.f)-1.f)
void simulation_step( std::vector<Particle*> pVector, float dt )
{
	int ii, size = pVector.size();
	
	for(ii=0; ii<size; ii++)
	{
		pVector[ii]->m_Position += dt*pVector[ii]->m_Velocity;
		pVector[ii]->m_Velocity = DAMP*pVector[ii]->m_Velocity + Vec2f(RAND,RAND) * 0.005f;
	}

}

