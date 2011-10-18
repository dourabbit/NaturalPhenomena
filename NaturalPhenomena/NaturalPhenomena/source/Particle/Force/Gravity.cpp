#include "Gravity.h"
#include <GL/glut.h>

Gravity::Gravity(Particle *p1) :
  m_p1(p1) {

	  p1->Forces.push_back(this);
  }

void Gravity::draw()
{
  glBegin( GL_LINES );
  glColor3f(0.6, 0.7, 0.8);
  glVertex3f( m_p1->m_Position[0], m_p1->m_Position[1], m_p1->m_Position[2]);
  glColor3f(0.6, 0.7, 0.8);
  glVertex3f( m_p1->m_Position[0], m_p1->m_Position[1]-0.5f, m_p1->m_Position[2] );
  glEnd();
}

void Gravity::gradient(DATA &x) 
{
	//return make_vector(0.0f, 0.0f, 0.0f);
}
void Gravity::gradientDot(DATA &x) 
{
	//return make_vector(0.0f, 0.0f, 0.0f);
}

void Gravity::apply_force()
{
	this->m_p1->m_ForceAccumulator += 
		make_vector(0.0,-9.8*this->m_p1->m_Mass,0.0);

}

Vector<DATA,3> Gravity::guessForce(Vector<DATA,3> &position, Particle &partilce)
{
	Vector<DATA,3> result =make_vector(0.0,-9.8*this->m_p1->m_Mass,0.0);
	return result;
}