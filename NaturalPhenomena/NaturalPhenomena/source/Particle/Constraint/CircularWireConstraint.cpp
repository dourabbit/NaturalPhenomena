#include "CircularWireConstraint.h"
#include <GL/glut.h>
#include <math.h>
#define PI 3.1415926535897932384626433832795

static void draw_circle(const Vector<DATA,3> & vect, float radius)
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

CircularWireConstraint::CircularWireConstraint(Particle *p, const Vector<DATA,3> & center, const double radius) :
	Constraint(p), m_center(center), m_radius(radius) {}

void CircularWireConstraint::draw()
{
	draw_circle(m_center, m_radius);
}

void CircularWireConstraint::gradient(DATA* pdx)
{
	////C(x,y)= sin(x)+cos(y)+0z-1;
	////J=[cos(x),-sin(y),0];

	//for(int i = 0 ; i<3; i++)
		//printf("\npos: %f,",pdx[i]);

	/*pdx[0] = cos(pdx[0]);
	pdx[1] = -sin(pdx[1]);
	pdx[2] = 0;*/
	//
	//

	//for(int i = 0 ; i<3; i++)
	//	printf("\nGradient: %f",pdx[i]);*/

	//Gradient
	for(int i = 0 ; i<3; i++)
	 pdx[i]= pdx[i];

}


void CircularWireConstraint::gradientDot(DATA* pddx)
{
	////dJ=[-sin(x)*dx, -cos(y)*dy,0]
	/*pddx[0] = -sin(pddx[0]);
	pddx[1] = -cos(pddx[1]);
	pddx[2] = 0;*/


	//Gradient
	for(int i = 0 ; i<3; i++)
		pddx[i]= pddx[i];
	
	/*for(int i = 0 ; i<3; i++)
		printf("\n??:%f",pddx[i]);*/

}