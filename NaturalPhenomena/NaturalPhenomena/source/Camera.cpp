#include "camera.h"
#include "math.h"
#include <iostream>
#include "windows.h"


#define SQR(x) (x*x)

#define NULL_VECTOR Vec3f(0.0f,0.0f,0.0f)
//
//SF3dVector F3dVector ( GLfloat x, GLfloat y, GLfloat z )
//{
//	SF3dVector tmp;
//	tmp.x = x;
//	tmp.y = y;
//	tmp.z = z;
//	return tmp;
//}
//
//GLfloat GetF3dVectorLength( SF3dVector * v)
//{
//	return (GLfloat)(sqrt(SQR(v->x)+SQR(v->y)+SQR(v->z)));
//}
//
//SF3dVector Normalize3dVector( SF3dVector v)
//{
//	SF3dVector res;
//	float l = GetF3dVectorLength(&v);
//	if (l == 0.0f) return NULL_VECTOR;
//	res.x = v.x / l;
//	res.y = v.y / l;
//	res.z = v.z / l;
//	return res;
//}
//
//SF3dVector operator+ (SF3dVector v, SF3dVector U)
//{
//	SF3dVector res;
//	res.x = v.x+U.x;
//	res.y = v.y+U.y;
//	res.z = v.z+U.z;
//	return res;
//}
//SF3dVector operator- (SF3dVector v, SF3dVector U)
//{
//	SF3dVector res;
//	res.x = v.x-U.x;
//	res.y = v.y-U.y;
//	res.z = v.z-U.z;
//	return res;
//}
//
//
//SF3dVector operator* (SF3dVector v, float r)
//{
//	SF3dVector res;
//	res.x = v.x*r;
//	res.y = v.y*r;
//	res.z = v.z*r;
//	return res;
//}
//
//SF3dVector CrossProduct (SF3dVector * U, SF3dVector * v)
//{
//	SF3dVector resVector;
//	resVector.x = U->y*v->z - U->z*v->y;
//	resVector.y = U->z*v->x - U->x*v->z;
//	resVector.z = U->x*v->y - U->y*v->x;
//
//	return resVector;
//}
//float operator* (SF3dVector v, SF3dVector U)	//dot product
//{
//	return v.x*U.x+v.y*U.y+v.z*U.z;
//}




/***************************************************************************************/

CCamera::CCamera()
{
	//Init with standard OGL values:
	Position = make_vector< float > (0.0, 0.0,	5.0);
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

	gluPerspective(60.0, 1.0, 1.5, 20.0);
	
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