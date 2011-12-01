#pragma once

//#include <gfx/vec3.h>
#include <glut.h>
#include <Vector\Vector.hpp>
#include <vector>
#include <Particle\Force\Force.hpp>
//#include <Particle\Constraint\Constraint.hpp>
#include <common.h>
class Constraint;
class Particle{


public:

	Particle(int ParticleID,const Vector<DATA,3> & ConstructPos,
		const DATA &mass,const GLint &slices,
		const GLint stacks,const GLdouble &radius );
	virtual ~Particle(void);

	int ParticleID;
	void reset();
	void draw();

	Vector<DATA,3> m_ConstructPos;
	Vector<DATA,3> m_Position;
	Vector<DATA,3> m_Velocity;
	DATA m_Mass;
	Vector<DATA,3> m_ForceAccumulator;

	std::vector<Force*> Forces;
	Constraint* pConstraint;
private:
	GLdouble _radius;
    GLint _slices;
	GLint _stacks;

};

