#include "SpringForce.hpp"
using std::string;

using std::ostringstream;

using std::cout;
using std::cerr;
using std::endl;
SpringForce::SpringForce(Particle *p1, Particle *p2, double dist, float *ks, float *kd) : 
m_p1(p1), m_p2(p2), m_dist(dist)
{
	m_ks= ks;
	m_kd= kd;
}

void SpringForce::draw()
{
	glBegin( GL_LINES );
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f( m_p1->m_Position.x, m_p1->m_Position.y, m_p1->m_Position.z);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f( m_p2->m_Position.x, m_p2->m_Position.y, m_p2->m_Position.z);
	glEnd();
}

void SpringForce::apply_force()
{
	//Spring Force
	Vector<float,3> force;
	Vector<float,3> direction= m_p2->m_Position-m_p1->m_Position;
	float length=length_squared(direction);
	direction=normalize(direction);
	force= *m_ks*((length-m_dist)*direction);

	//Spring Damp
	float dampFactor=-1* (*m_kd)*dot_product(m_p1->m_Velocity-m_p2->m_Velocity,direction);
	force += dampFactor*direction;

	//cerr <<*m_ks<< endl;

	m_p1->m_ForceAccumulator+=force;
	m_p2->m_ForceAccumulator+=-force;

}

