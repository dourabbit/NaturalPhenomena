#include "camera.h"
#include "math.h"
#include <iostream>
#include "windows.h"


#define SQR(x) (x*x)

#define NULL_VECTOR Vec3f(0.0f,0.0f,0.0f)
CCamera::CCamera()
{
	this->Reset();
}

void CCamera::Reset()
{
	
	//Init with standard OGL values:
	Position = make_vector< float > (0.0, 0.0,	20.0);
	ViewDir = make_vector< float >( 0.0, 0.0, -1.0);
	RightVector = make_vector< float > (1.0, 0.0, 0.0);
	UpVector = make_vector< float > (0.0, 1.0, 0.0);

	//Only to be sure:
	RotatedX = RotatedY = RotatedZ = 0.0;

}
void CCamera::Move (Vector<float,3> Direction)
{
	Position = Position + Direction;
}

void CCamera::RotateX (GLfloat Angle)
{
	RotatedX += Angle;
	
	float d = cos(Angle*PIdiv180);
	Vector<float,3> a= ViewDir*d;
	d=sin(Angle*PIdiv180);
	Vector<float,3> b= UpVector*d;

	//Rotate viewdir around the right vector:
	ViewDir = normalize(a+b);

	//now compute the new UpVector (by cross product)
	UpVector = cross_product(ViewDir, RightVector);
	UpVector *= -1;

	
}

void CCamera::RotateY (GLfloat Angle)
{
	RotatedY += Angle;
	
	//Rotate viewdir around the up vector:

	float d=cos(Angle*PIdiv180);
	Vector<float,3> a=ViewDir*d;
	d=sin(Angle*PIdiv180);
	Vector<float,3> b=RightVector*d;

	ViewDir = normalize(a-b);




	//now compute the new RightVector (by cross product)
	RightVector = cross_product(ViewDir, UpVector);
}

void CCamera::RotateZ (GLfloat Angle)
{
	RotatedZ += Angle;
	
	float d=cos(Angle*PIdiv180);
	Vector<float,3> a =RightVector*d;
	d=sin(Angle*PIdiv180);
	Vector<float,3> b = UpVector*d;
	//Rotate viewdir around the right vector:
	RightVector = normalize(a+b );

	//now compute the new UpVector (by cross product)
	Vector<float,3> c = cross_product(ViewDir, RightVector);
	UpVector = -c;
}

void CCamera::Render( int win_x, int win_y )
{
	glViewport ( 0, 0, win_x, win_y );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ();

	gluPerspective(70.0, 1.0, 0.1, 200.0);
	
	//The point at which the camera looks:
	Vector<float,3> ViewPoint = Position+ViewDir;

	//as we know the up vector, we can easily use gluLookAt:
	gluLookAt(	Position[0],Position[1],Position[2],
				ViewPoint[0],ViewPoint[1],ViewPoint[2],
				UpVector[0],UpVector[1],UpVector[2]);


	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );

	
}

void CCamera::MoveForward( GLfloat Distance )
{
	Position = Position + (ViewDir*-Distance);
}

void CCamera::StrafeRight ( GLfloat Distance )
{
	Position = Position + (RightVector*Distance);
}

void CCamera::MoveUpward( GLfloat Distance )
{
	Position = Position + (UpVector*Distance);
}