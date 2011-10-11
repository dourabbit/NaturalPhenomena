#pragma once

//#include <gfx/vec3.h>
#include <glut.h>
#include <Vector\Vector.hpp>
class Particle{


public:

	Particle(const Vector<float,3> & ConstructPos,const GLint &slices,const GLint stacks,const GLdouble &radius );
	virtual ~Particle(void);

	void reset();
	void draw();

	Vector<float,3> m_ConstructPos;
	Vector<float,3> m_Position;
	Vector<float,3> m_Velocity;
	float m_Mass;
	Vector<float,3> m_ForceAccumulator;

private:
	GLdouble _radius;
    GLint _slices;
	GLint _stacks;

};

