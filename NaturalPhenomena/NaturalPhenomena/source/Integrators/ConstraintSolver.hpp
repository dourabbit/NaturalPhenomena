
#ifndef _CONSTRAINTSOLVER_HPP_
#define _CONSTRAINTSOLVER_HPP_
#include <common.h>
#include <Integrators\linearSolver.h>
#include <Integrators\Solver.hpp>
#include <Vector\MatrixP.hpp>
class Solver;
class ConstraintSolver:public implicitMatrix{
public: 

		ConstraintSolver(Solver* pSolver);
		~ConstraintSolver();
		Solver* pSolver;

		

		int Row;
		int Col;
		
		int* ParticleIDs;
		virtual void Initialize();
		virtual void Solve();
		virtual void matVecMult(double x[], double r[]);

private:
		bool _isInitialized;

		MatrixP _leftHandSide_m;
		MatrixP _rightHandSide_m;

		//Row: constraints.size()
		//Col: numOfParti * 3
		DATA* _J;//Row*Col
		DATA* _JT;//Col*Row
		DATA* _WeightInverse;//Col*1
		DATA* _JDot;//Row*Col
		DATA* _qDot;//Col*1
		DATA* _QForces;//Col*1
		DATA* _Lamada;//Row*1
    
};
#endif