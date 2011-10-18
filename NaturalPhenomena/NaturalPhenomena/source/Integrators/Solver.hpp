//
//
#ifndef _SOLVER_HPP_
#define _SOLVER_HPP_

#define DAMP 0.98f
#define RAND (((rand()%2000)/1000.f)-1.f)

//#define NumDim 3
//#ifndef NUMOFCol
//#error "Define the num of parti"
//#endif

#include <Particle\Particle.h>
#include <Vector\Matrix.hpp>

#include <vector>
#include <Integrators\Integrator.hpp>
#include <Particle\Force\Force.hpp>
#include <Particle\Constraint\Constraint.hpp>
#include <Integrators\ConstraintSolver.hpp>
#include <common.h>
class Integrator;
class ConstraintSolver;
class Solver{
public: 

		Solver();
		~Solver();
		void Initialize(const int &numOfParti, std::vector<Particle*> pVector, 
						std::vector<Force*> pForces, std::vector<Constraint*> pConstraints);
		void update(DATA dt, 
					Integrator* integrator);
		void getState(DATA* phaseSpace);
		void setState(DATA* phaseSpace);
		int _numOfParti;
		std::vector<Force*> _pForces;
		std::vector<Constraint*> _pConstraints;
		std::vector<Particle*> _pParti;
		
		int DIM;
		void getDerivative(DATA* dst);
private:
	
	
	std::vector<DATA*> _px;
	std::vector<DATA*> _pDx;
	ConstraintSolver* _constSolver;
	//DATA* _pJacobi;
};
#endif