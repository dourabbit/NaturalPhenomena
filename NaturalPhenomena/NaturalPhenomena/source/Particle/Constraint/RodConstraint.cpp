#include "RodConstraint.h"
#include <GL/glut.h>

RodConstraint::RodConstraint(Particle *p1, Particle * p2, DATA dist) :
  Constraint(p1), m_p2(p2), m_dist(dist) {
	
  
  }

void RodConstraint::draw()
{
  glBegin( GL_LINES );
  glColor3f(0.8, 0.7, 0.6);
  glVertex3f( m_p1->m_Position[0], m_p1->m_Position[1], m_p1->m_Position[2]);
  glColor3f(0.8, 0.7, 0.6);
  glVertex3f( m_p2->m_Position[0], m_p2->m_Position[1], m_p2->m_Position[2]);
  glEnd();

}

void RodConstraint::gradient(DATA* pdx)
{
	//Gradient
	/*for(int i = 0 ; i<3; i++)
	 pdx[i]= pdx[i];
*/
	Vector<DATA,3> d = 2*this->m_p1->m_Position-this->m_p2->m_Position;
	pdx[0]= d.x;
	pdx[1]= d.y;
	pdx[2]= d.z;

}


void RodConstraint::gradientDot(DATA* pddx)
{
	//Gradient
	/*for(int i = 0 ; i<3; i++)
		pddx[i]= pddx[i];*/



	Vector<DATA,3> d = 2*this->m_p1->m_Velocity-this->m_p2->m_Velocity;
	pddx[0]= d.x;
	pddx[1]= d.y;
	pddx[2]= d.z;

}

DATA RodConstraint::eval()
{
	return length(m_p1->m_Position - m_p2->m_Position) - m_dist;
}

DATA RodConstraint::evalDeriv()
{
	return 2.0 * (m_p1->m_Position[0] - m_p2->m_Position[0]) * (m_p1->m_Velocity[0] - m_p2->m_Velocity[0])
		+ 2.0 * (m_p1->m_Position[1] - m_p2->m_Position[1]) * (m_p1->m_Velocity[1] - m_p2->m_Velocity[1]);
}