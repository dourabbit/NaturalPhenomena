#pragma once
//#include "VectorField.h"

#include "common.h";
class ScalarField
{
public:
	ScalarField(int a_NumCells);
	ScalarField(ScalarField *CopyField);
	virtual ~ScalarField(void);

	//void	TimeStep(ScalarField *a_SrcField, VectorField *VelocityField);
	//void	AddField(ScalarField *a_SrcField);
	//void	Advect(ScalarField *prevScalarField, VectorField *velField);
	//void	setBoundary(float * x);
	DATA	&operator[](int i)       { return m_Field[i]; }
	
	int		m_NumCells;
	DATA	*m_Field;
	//DATA	m_Viscosity;
	//DATA	m_Dt;
};
