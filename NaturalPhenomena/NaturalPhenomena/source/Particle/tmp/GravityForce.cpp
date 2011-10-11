#include "GravityForce.hpp"
#define G 9.8f;
GravityForce::GravityForce(Particle *p1) : 
m_p1(p1)
{




}

void GravityForce::draw()
{
}

void GravityForce::apply_force()
{

	float g= m_p1->m_Mass * G;
	Vector3f force= make_vector(0.0f,-g,0.0f);
	m_p1->m_ForceAccumulator += force;
}

