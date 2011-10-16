
#ifndef _CONSTRAINTSOLVER_HPP_
#define _CONSTRAINTSOLVER_HPP_
#include <common.h>
#include <Integrators\linearSolver.h>

class Solver;
class ConstraintSolver:public implicitMatrix{
public: 

		ConstraintSolver(Solver* pSolver);
		~ConstraintSolver();
		Solver* pSolver;
		virtual void Solve();
		virtual void matVecMult(double x[], double r[]);
};
#endif