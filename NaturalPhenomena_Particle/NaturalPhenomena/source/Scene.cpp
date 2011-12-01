
#include <Scene.hpp>



Scene::Scene(std::string sceneNm ,static std::vector<Particle*> parti,static std::vector<Force*> forces,
		static std::vector<Constraint*> constraints)
{
	this->SceneNm = sceneNm;
	this->pParti = parti;
	this->pForces = forces;
	this->pConstraints = constraints;
};
Scene::~Scene(){
		for(int ii=0; ii<pParti.size();ii++)
			delete pParti[ii];
		for(int ii=0; ii<pForces.size();ii++)
			delete pForces[ii];
		for(int ii=0; ii<pConstraints.size();ii++)
			delete pConstraints[ii];
	
};

