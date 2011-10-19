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

	Cloth(std::vector<Particle*> &parti,std::vector<Force*> &forces);
	~Cloth();
	
	DATA DRAG;
	DATA Damp;
	DATA KS;
	
	DATA WindStrength;
	 Vector<DATA,3> WindDir;

	//void ClothGen(int U, int V,  Vector<DATA,3> UOffset,  Vector<DATA,3> VOffset,  Vector<DATA,3> originalPos);
	void reset();
	void draw(std::vector<Particle*> &pVector,std::vector<Force*> &fVector);
	int U;
	int V;
	
private:
	
};

#endif

