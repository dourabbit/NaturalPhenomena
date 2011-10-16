#include "CircularWireConstraint.h"
#include <GL/glut.h>

#define PI 3.1415926535897932384626433832795

static void draw_circle(const Vector<float,3> & vect, float radius)
{
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0,1.0,0.0); 
	for (int i=0; i<360; i=i+18)
	{
		float degInRad = i*PI/180;
		glVertex3f(vect[0]+cos(degInRad)*radius,vect[1]+sin(degInRad)*radius,0);
	}
	glEnd();
}

CircularWireConstraint::CircularWireConstraint(Particle *p, const Vector<float,3> & center, const double radius) :
	m_p(p), m_center(center), m_radius(radius),Constraint() {}

void CircularWireConstraint::draw()
{
	draw_circle(m_center, m_radius);
}

void CircularWireConstraint::partialDx(float &x)
{
	//C(x,y)= sin(x)+cos(y)-1;
	//J=[cos(x),-sin(y)];
	//dJ=
	 
}


void CircularWireConstraint::partialDDx(float &x)
{


}