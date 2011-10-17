
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

		DATA* Jacobi;
		DATA* Weight;
		DATA* Jacobi_T;


		virtual void Solve();
		virtual void matVecMult(double x[], double r[]);
};
#endif