#ifndef _SPRINGFROCE_HPP_
#define _SPRINGFROCE_HPP_

#include "Particle.hpp"
#include "Force.hpp"
class SpringForce : public Force
{

public:
	SpringForce(Particle *p1, Particle *p2, double dist,float ks, float kd);

	virtual void draw();
	virtual void apply_force();
private:
	
	Particle *const m_p1; //particle1
	Particle *const m_p2; //particle2
	float const m_dist;  //rest length
	float m_ks;
	float m_kd; //spring strength constants
};
#endif
