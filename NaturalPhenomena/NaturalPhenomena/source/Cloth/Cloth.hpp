#ifndef _CLOTH_HPP_
#define _CLOTH_HPP_

#include <Particle/Particle.h>
#include <Particle/Force/Force.hpp>
#include <Particle/Force/SpringForce.h>
#include <Particle/Force/Gravity.h>
//#include <Particle/Force/PointConstrain.hpp>
#include <vector>

class Cloth{

public:

	Cloth();
	~Cloth();
	
	float DRAG;
	float DAMP;
	float KS;
	
	float WindStrength;
	Vector3f WindDir;

	void ClothGen(int U, int V, Vector3f UOffset, Vector3f VOffset, Vector3f originalPos);
	void reset();
	void draw();
	
	void simulation_step(float elapsedTime);
	/*void forward_euler(Vector3f acc, Particle* pParticle, float elaspedTime);
	void verlet_noVelocity(Vector3f acc, Particle* pParticle, float elaspedTime);*/


private:
	int U;
	int V;
};

#endif

