#include "Particle.h"
#include <GL/glut.h>

Particle::Particle(int particleID,const Vector<DATA,3> & ConstructPos,const DATA &mass,
	const GLint &slices,const GLint stacks,const GLdouble &radius ) :
	m_ConstructPos(ConstructPos), 
	m_Position(make_vector< DATA >(0.0, 0.0,0.0)), 
	m_Velocity(make_vector< DATA >(0.0, 0.0,0.0)),
	m_ForceAccumulator(make_vector< DATA >(0.0, 0.0,0.0)),
	m_Mass(mass)
{
	this->_radius = radius;
	this->_slices= slices;
	this->_stacks= stacks; 
	this->Forces.clear(); 
	this->pConstraint=NULL;

	this->ParticleID = particleID;
}

Particle::~Particle(void)
{
	int ii, size = Forces.size();

	for(ii=0; ii<size; ii++){
		delete Forces[ii];
	}
}

void Particle::reset()
{
	m_Position = m_ConstructPos;
	m_Velocity = make_vector(0.0, 0.0,0.0);
}
void Particle::draw()
{
	const double h = 0.03;
	glColor3f(0.5f, 1.f, 1.f); 
	
	/*glVertex3f(m_Position[0]-h/2.0, m_Position[1]-h/2.0);
	glVertex3f(m_Position[0]+h/2.0, m_Position[1]-h/2.0);
	glVertex3f(m_Position[0]+h/2.0, m_Position[1]+h/2.0);
	glVertex3f(m_Position[0]-h/2.0, m_Position[1]+h/2.0);*/
	glPushMatrix();

	/*glTranslatef(m_Position[0],m_Position[1],m_Position[2]);*/
	glTranslatef(m_Position[0],m_Position[1],m_Position[2]);

	glBegin(GL_QUADS);
	glutSolidSphere(_radius, _slices, _stacks);
	glEnd();
	glPopMatrix();
}
