#include "Particle.hpp"

Particle::Particle(const Vector3f & ConstructPos) 
{
	m_ConstructPos = make_vector< float >(ConstructPos.x, ConstructPos.y, ConstructPos.z);
	m_Position = make_vector< float >(0.0f, 0.0f, 0.0f);
	prevPosition = make_vector< float >(0.0f, 0.0f, 0.0f);
	m_Velocity = make_vector< float >(0.0f, 0.0f, 0.0f);
	m_ForceAccumulator = make_vector< float >(0.0f, 0.0f, 0.0f);
	m_Mass = 3.0f;
}

Particle::~Particle(void)
{
}

void Particle::reset()
{
	m_Position = m_ConstructPos;
	m_Velocity = make_vector< float >(0.0f, 0.0f, 0.0f);
	this->prevPosition =m_ConstructPos;
	this->m_ForceAccumulator= make_vector< float >(0.0f, 0.0f, 0.0f);
}

void Particle::draw()
{
	const double h = 0.3;
	glColor3f(1.f, 0.f, 0.f);
	glBegin(GL_QUADS);
	glVertex3f(m_Position.x-h/2.0, m_Position.y-h/2.0, m_Position.z);
	glVertex3f(m_Position.x+h/2.0, m_Position.y-h/2.0, m_Position.z);
	glVertex3f(m_Position.x+h/2.0, m_Position.y+h/2.0, m_Position.z);
	glVertex3f(m_Position.x-h/2.0, m_Position.y+h/2.0, m_Position.z);
	glEnd();
}

void Particle::clearForce()
{
	this->m_ForceAccumulator = make_vector< float >(0.0f, 0.0f, 0.0f);
}

