#ifndef _WINDFOROCE_HPP_
#define _WINDFOROCE_HPP_

#include "Particle.hpp"
#include "Force.hpp"
class WindForce : public Force
{

public:
	WindForce(Particle *p1, Particle *p2,Particle *p3,Particle *p4,float* pWindStrength, Vector3f* pWindDir);

	virtual void draw();
	virtual void apply_force();
	Vector3f Wind;
	
private:
	
	Particle *const m_p1; //particle1
	Particle *const m_p2; //particle2
	Particle *const m_p3; //particle3
	Particle *const m_p4; //particle4

	Vector3f*  _pWindDir;
	float*  _pWindStrength;
};
#endif
