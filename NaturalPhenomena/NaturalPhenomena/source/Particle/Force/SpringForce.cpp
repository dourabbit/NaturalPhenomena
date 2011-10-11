#include "SpringForce.h"
#include <GL/glut.h>

SpringForce::SpringForce(Particle *p1, Particle * p2, double dist, double ks, double kd) :
  m_p1(p1), m_p2(p2), m_dist(dist), m_ks(ks), m_kd(kd) {
	
		m_ks= ks;
		m_kd= kd;
  
  
  }

void SpringForce::draw()
{
  glBegin( GL_LINES );
  glColor3f(0.6, 0.7, 0.8);
  glVertex3f( m_p1->m_Position[0], m_p1->m_Position[1], m_p1->m_Position[2]);
  glColor3f(0.6, 0.7, 0.8);
  glVertex3f( m_p2->m_Position[0], m_p2->m_Position[1], m_p2->m_Position[2]  );
  glEnd();
}



void SpringForce::apply_force()
{
	//Spring Force
	Vector<float,3> force;
	_direction= m_p2->m_Position-m_p1->m_Position;
	double length=length_squared(_direction);
	_direction=normalize(_direction);
	force= (m_ks)*((length-m_dist)*_direction);
	
	//Spring Damp
	//float dampFactor=-1* (m_kd)*dot_product(m_p1->m_Velocity-m_p2->m_Velocity,direction);
	float dampFactor=-1* (m_kd)*((m_p1->m_Velocity-m_p2->m_Velocity)*_direction);
	
	force += dampFactor*_direction;

	//cerr <<*m_ks<< endl;

	m_p1->m_ForceAccumulator+=force;
	m_p2->m_ForceAccumulator+=-force;

}


Vector<float,3> SpringForce::partialDx()
{
	Vector<float,3> result = make_vector(0.0f,0.0f,0.0f);
	result.x = this->m_ks * this->_direction*make_vector(1.0f,0.0f,0.0f);
	result.y = this->m_ks * this->_direction*make_vector(0.0f,1.0f,0.0f);
	result.z = this->m_ks * this->_direction*make_vector(0.0f,0.0f,1.0f);

	return result;
}

Vector<float,3> SpringForce::partialDDx()
{
	Vector<float,3> result = make_vector(0.0f,0.0f,0.0f);
	result.x = -this->m_kd * this->_direction*make_vector(1.0f,0.0f,0.0f);
	result.y = -this->m_kd * this->_direction*make_vector(1.0f,0.0f,0.0f);
	result.z = -this->m_kd * this->_direction*make_vector(1.0f,0.0f,0.0f);
}