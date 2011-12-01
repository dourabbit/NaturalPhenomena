#pragma once
//#include <gfx/vec2.h>
#include "Vector.hpp"
#include "common.h"
#include "ScalarField.h"
class VectorField
{
public:
	//VectorField(int a_NumCells, float a_Viscosity, float a_Dt);
	VectorField(VectorField *CopyField);
	
	/*void	TimeStep(VectorField *a_SrcField, VectorField *VelocityField);
	void	AddField(VectorField *a_SrcField);
*/
	//Vec2f	&operator[](int i)       { return m_Field[i]; }
	Vector<DATA,2>  &operator[](int i) {return m_Field[i];}

	VectorField(int a_NumCells);
	virtual ~VectorField(void);
	
 	void GetUV(ScalarField* u,ScalarField* v);

	int		m_NumCells;
	//Vec2f	*m_Field;
	Vector<DATA,2> *m_Field;
	
	/*float	m_Viscosity;
	float	m_Dt;*/
};
