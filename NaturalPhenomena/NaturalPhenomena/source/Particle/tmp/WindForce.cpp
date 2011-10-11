#include "WindForce.hpp"
using std::string;
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
using std::ostringstream;

using std::cout;
using std::cerr;
using std::endl;
WindForce::WindForce(Particle *p1, Particle *p2,Particle *p3,Particle *p4, float* pWindStrength, Vector3f* pWindDir) : 
m_p1(p1), m_p2(p2), m_p3(p3),  m_p4(p4)
{
	_pWindDir= pWindDir;
	_pWindStrength= pWindStrength;

}

void WindForce::draw()
{
	glBegin( GL_LINES );
	
	glVertex3f(m_p1->m_Position.x,m_p1->m_Position.y,m_p1->m_Position.z);
	glVertex3f( Wind.x/10+m_p1->m_Position.x,
				Wind.y/10+m_p1->m_Position.y,
				Wind.z/10+m_p1->m_Position.z);
	glEnd();
}

void WindForce::apply_force()
{
	Vector3f dirA= m_p2->m_Position -  m_p1->m_Position;
	dirA = normalize(dirA);
	Vector3f dirB= m_p3->m_Position -  m_p2->m_Position;
	dirB = normalize(dirB);
	Vector3f dirC= m_p4->m_Position -  m_p3->m_Position;
	dirC = normalize(dirC);
	Vector3f dirD= m_p4->m_Position -  m_p1->m_Position;
	dirD = normalize(dirD);

	Vector3f dir = cross_product(dirA, dirD)+ cross_product(dirA, dirB)+ cross_product(dirB, dirC)+ cross_product(dirC, dirD);

	dir = normalize(dir);

	float coeffient=dir* *_pWindDir;

	Wind =  coeffient* *_pWindStrength * *_pWindDir;

	m_p1->m_ForceAccumulator += Wind;
	m_p2->m_ForceAccumulator += Wind;
	m_p3->m_ForceAccumulator += Wind;
	m_p4->m_ForceAccumulator += Wind;
}

