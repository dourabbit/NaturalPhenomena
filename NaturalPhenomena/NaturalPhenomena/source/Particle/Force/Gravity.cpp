#include "Gravity.h"
#include <GL/glut.h>

Gravity::Gravity(Particle *p1) :
  m_p1(p1) {
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



void Gravity::apply_force()
{


}