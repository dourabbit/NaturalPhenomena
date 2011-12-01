#include "ScalarField.h"

#include <math.h>

#define CREATE_DIM1  (new DATA[(a_NumCells+2)*(a_NumCells+2)])
#define CREATE_DIM2  (new DATA[(CopyField->m_NumCells+2)*(CopyField->m_NumCells+2)])
#define CREATE_DIM   (new DATA[(m_NumCells+2)*(m_NumCells+2)])
#define IX_DIM(i, j) ((i)+(m_NumCells+2)*(j)) 
#define ITER_DIM     for(int i = 1; i <= m_NumCells; i++){ for(int j = 1; j <= m_NumCells; j++){
#define ENDITER_DIM  }}

ScalarField::ScalarField(int a_NumCells)
:m_Field(CREATE_DIM1), m_NumCells(a_NumCells)
{
	for(int i = 0; i < (m_NumCells+2)*(m_NumCells+2); i++) 
		m_Field[i] = 0.0f;
}

ScalarField::ScalarField(ScalarField *CopyField)
: m_NumCells(CopyField->m_NumCells), m_Field(CREATE_DIM2)
{
	for(int i = 0; i < (m_NumCells+2)*(m_NumCells+2); i++) 
		m_Field[i] = (*CopyField)[i];
}

ScalarField::~ScalarField(void)
{
	delete m_Field;
}

//void
//ScalarField::TimeStep(ScalarField *a_SrcField, VectorField *VelocityField)
//{
//	//AddField(a_SrcField);
//	//Advection(VelocityField);
//
//	///*****************************************/
//	///************* DELETE THIS ***************/
//	//static float change = .001f;
//	//change += .001f;
//	//float schange = 0.005f*sin(change);
//	//schange += 1.f;
//
//	//ITER_DIM
//	//	m_Field[IX_DIM(i,j)] *=	schange;
//	//ENDITER_DIM
//	///************* DELETE THIS ***************/
//	///*****************************************/
//
//	this->AddField(a_SrcField);
//	this->Advect(a_SrcField,VelocityField);
//	
//
//}
//void ScalarField::Advect(ScalarField *prevScalarField, VectorField *velField)
//{
//	int i, j, rowLeftBoundary, colDownBoundary, rowRightBoundary, colUpBoundary;
//	float x, y, s0, t0, s1, t1;
//	
//	
//	ITER_DIM
//		x = i-this->m_Dt *m_NumCells* velField->m_Field[IX_DIM(i,j)][0];
//		y = j-this->m_Dt*m_NumCells* velField->m_Field[IX_DIM(i,j)][1];
//		if(x<0.5f) x = 0.5f;
//		if(y<0.5f) y=0.5f;
//		if(x>m_NumCells+0.5f) x = m_NumCells+0.5f;
//		if(y>m_NumCells+0.5f) y= m_NumCells+0.5f;
//
//		rowLeftBoundary = (int)x;
//		rowRightBoundary = rowLeftBoundary +1;
//		colDownBoundary = (int)y;
//		colUpBoundary = colDownBoundary+1;
//
//		s1 = x-rowLeftBoundary; s0 = 1-s1; t1 = y-colDownBoundary; t0 = 1-t1;
//		this->m_Field[IX_DIM(i,j)] = 
//			s0*(this->m_Dt*m_NumCells*prevScalarField->m_Field[IX_DIM(rowLeftBoundary,colDownBoundary)]+
//			t1*prevScalarField->m_Field[IX_DIM(rowLeftBoundary,colUpBoundary)])+
//
//			s1*(t0*prevScalarField->m_Field[IX_DIM(rowRightBoundary,colDownBoundary)]+
//			t1*prevScalarField->m_Field[IX_DIM(rowRightBoundary,colUpBoundary)]);
//
//	ENDITER_DIM
//
//		this->setBoundary(this->m_Field);
//
//}
//
//void ScalarField::setBoundary(float * x)
//{
//	x[IX_DIM(0  ,0  )] = 0.5f*(x[IX_DIM(1,0  )]+x[IX_DIM(0  ,1)]);
//	x[IX_DIM(0  ,m_NumCells+1)] = 0.5f*(x[IX_DIM(1,m_NumCells+1)]+x[IX_DIM(0  ,m_NumCells)]);
//	x[IX_DIM(m_NumCells+1,0  )] = 0.5f*(x[IX_DIM(m_NumCells,0  )]+x[IX_DIM(m_NumCells+1,1)]);
//	x[IX_DIM(m_NumCells+1,m_NumCells+1)] = 0.5f*(x[IX_DIM(m_NumCells,m_NumCells+1)]+x[IX_DIM(m_NumCells+1,m_NumCells)]);
//}
//void ScalarField::AddField(ScalarField *a_SrcField)
//{
//	for(int i=0; i<((m_NumCells+2)*(m_NumCells+2)); i++) {
//		m_Field[i] += m_Dt*((*a_SrcField)[i]);
//	}
//}

