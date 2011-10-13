//#ifndef _COMMON_HPP_
//#define _COMMON_HPP_
//#include <Vector/Vector.hpp>
//#include <Particle/Particle.h>
//#include <Integrators\Integrator.hpp>
//#include <vector>
////
////#define DAMP 0.98f
////#define RAND (((rand()%2000)/1000.f)-1.f)
////
//////#define NumDim 3
//////#ifndef NUMOFCol
//////#error "Define the num of parti"
//////#endif
////
////#include <Particle\Particle.h>
////#include <Vector\Matrix.hpp>
////
////#include <vector>
//////#include <Integrators\Integrator.hpp>
////#include <Particle\Force\Force.hpp>
////#include <Particle\Constraint\Constraint.hpp>
////
////
////class Solver{
////public: 
////
////		Solver();
////		~Solver();
////		void Initialize(const int _numOfParti, std::vector<Particle*> pVector,
////						std::vector<Force*> pForces, std::vector<Constraint*> pConstraints);
////		void update( std::vector<Particle*> pVector, float dt, Integrator* integrator);
////		float*  getState();
////		int _numOfParti;
////private:
////		std::vector<Particle*> _pParti;
////		std::vector<Force*> _pForces;
////		std::vector<Constraint*> _pConstraints;
////		void getDerivative();
////		float* _px;
////		float* _pDx;
////		float* _pJacobi;
////};
//class Solver;
//class ImplicitIntegrator: public Integrator{
//
//public:
//
//	ImplicitIntegrator(Solver* solver);
//	
//	virtual void Integrate(std::vector<Particle*> pVector, float elaspedTime,float* px,float* pDx);
//
//	//Cloth* _cloth;
//};
//#endif