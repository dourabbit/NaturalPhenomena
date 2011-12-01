#pragma once
//#include <gfx/vec2.h>
#include "common.h"
#include "VectorField.h";
#include "ScalarField.h";
#include "linearSolver.h";



class MacGrid2D
{



public:
	MacGrid2D(int N,DATA dt,DATA diff);
	virtual ~MacGrid2D(void);
	void draw(void);

	int	&operator[](int x)       
	{ 


	}

	void DensityTimeStep(ScalarField &source, ScalarField &result,VectorField &field);

	void advectScalar(ScalarField &result, ScalarField &source,VectorField &field);

	/*void advectVector(ScalarField &u,ScalarField &v,
				ScalarField &uField,ScalarField &vField);
*/

	void advectVector(VectorField &dest, VectorField &src);

	void InterpolateVel(DATA *uField,DATA *vField, 
					DATA x, DATA y,Vector<DATA,2> &result);
	
	void AddScalarField(ScalarField &a_SrcField, ScalarField &destination);
	/*
	void AddVectorField(ScalarField *u_SrcField,ScalarField *v_SrcField, 
		ScalarField *u_dest,ScalarField *v_dest);
	*/
	void AddVectorField(VectorField &src,VectorField &dest);

	void VelTimeStep(VectorField &src,VectorField &dest);

	void lin_solve (float * x, float * x0, float a, float c );
	void diffuse ( float * x, float * x0 );
	void DiffuseVector (VectorField &result, VectorField &src);

	//void set_VelBnd (ScalarField * u,ScalarField * v );
	void set_VelBnd (VectorField &field);
	void set_ScarBnd (DATA * x );

	//void project (ScalarField * u, ScalarField * v);
	void project (VectorField &dest);
	void Swap(ScalarField &a, ScalarField &b);
	void Swap(VectorField &a, VectorField &b);
	int		m_N;
	//ScalarField *DesityField;
	/*ScalarField *vField;
	ScalarField *uField;*/
	//VectorField *VectorField;
	DATA	m_Viscosity;
	DATA	m_Dt;
};
