#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include <Particle\Particle.h>
#include <Particle\Constraint\Constraint.hpp>
#include <Particle\Force\Force.hpp>

class Scene{
public:
	int IndexOfIntegrator;
	std::vector<Particle*> pParti;
	std::vector<Force*> pForces;
	std::vector<Constraint*> pConstraints;
	std::string SceneNm;
	Scene(std::string sceneNm ,static std::vector<Particle*> parti,static std::vector<Force*> forces,
		  static std::vector<Constraint*> constraints);
	
	~Scene();
};

#endif

