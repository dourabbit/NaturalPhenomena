#include "MacGrid2D.h"
#include <GL/glut.h>

#define I(col,row) ((col)+(m_N)*(row))
#define IX(col,row) ((col)+(m_N+2)*(row))
#define IXX(col,row) ((col)+(m_N+3)*(row))
#include <math.h>;


class projectMatrix:public implicitMatrix
{

public:
	int m_N;
	projectMatrix(int N)
	{
		this->m_N = N;
	}
	void index(int &index,int &i, int &j, double &result)
	{
		
		int c = (int)(index)%(m_N+2);
		int r = (int)((index-c)/(m_N+2));

			
		if(abs(r-j)==1&&c==i)
			result = 1;
		else if(abs(c-i)==1&&r==j)
			result = 1;
		else if(c==i&&r==j)
		{
			/*if((i==0&&j==0)||(i==m_N+1&&j==m_N+1)||(i==m_N+1&&j==0)||(i==0&&j==m_N+1))
				result= -2;
			else if(i==0||j==0||i==m_N+1||j==m_N+1)
				result= -3;
			else*/
				result = -4;
		}
		else 
			result =0;
		


	}

	virtual void matVecMult(double x[], double b[]){
		
		
		double * A = (double*)malloc((m_N+2)*(m_N+2)*(m_N+2)*(m_N+2)*sizeof(double));
		
		int ii=0;
		//printf("\n\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n\n");

		for(int k=0;k< (m_N+2)*(m_N+2);k++)
		{
		for(int j=0;j<(m_N+2);j++)
		{
			for(int i=0;i<(m_N+2);i++)
			{
				double a=0.0f;
				index(k,i,j,a);
				//printf("%f,\t",a);
				A[ii]=a;
				ii++;
				
			}
		}
		
			//printf("\n");
		}
		//printf("\n\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n\n");

		
		
	/*	for(int i=0;i< (m_N+2)*(m_N+2);i++)
		{
			for(int j=0;j< (m_N+2)*(m_N+2);j++)
			{
				if(i==j&&i==0)
					A[i+j*((m_N+2)*(m_N+2))]=0.1f;
				else
					A[i+j*((m_N+2)*(m_N+2))]=0.0f;
			}
		}
		*/
		/*printf("A:\n\n");
		for(int i=0;i< (m_N+2)*(m_N+2);i++)
		{
			for(int j=0;j< (m_N+2)*(m_N+2);j++)
			{
				printf("%f\t",A[i+j*((m_N+2)*(m_N+2))]);
			}
			printf("\n");
		}
*/

		for(int k=0;k< (m_N+2)*(m_N+2);k++)
		{
			b[k]=0.0f;//0.5f*x[k];

			
			ii=0;
			for(int i=0;i<(m_N+2)*(m_N+2);i++)
			{
				//printf("%f,\t",A[ii+k*(m_N+2)*(m_N+2)]);
				b[k]+=A[ii+k*(m_N+2)*(m_N+2)]*(x[ii]);
				ii++;
			}
			//printf("\n%f==?==%f\n",b[k],0.5f*x[k]);
		}
		
		delete[] A;
	}


};


MacGrid2D::MacGrid2D(int N,DATA dt,DATA diff)
	:m_N(N), m_Viscosity(diff), m_Dt(dt)
{


}
MacGrid2D::~MacGrid2D(){}
void MacGrid2D::advectScalar(ScalarField &result, ScalarField &source,
								VectorField &field)
{


	DATA x,y, s0,t0,s1,t1;
	int i0,j0,i1,j1;
	DATA t = this->m_Dt*0.001;
	for(int i=0;i<this->m_N+2;i++)
	{
		for(int j=0;j<this->m_N+2;j++)
		{
			x = i- t* (field[IX(i,j)].x+field[IX(i+1,j)].y)*0.5f;
			y = j- t* (field[IX(i,j)].y+field[IX(i,j+1)].y)*0.5f;


			//Set boundary
			if(x<0.5f) x=0.5f;
			if(y<0.5f) y=0.5f;
			if(x>m_N+0.5f) x=m_N+0.5f;
			if(y>m_N+0.5f) y=m_N+0.5f;

			i0 =(int)x;
			j0 =(int)y;
			i1 = i0+1;
			j1 = j0+1;

			s1 = x-i0; s0 = 1-s1; t1 = y-j0; t0 = 1-t1;
			result[IX(i,j)] = s0*(t0*source[IX(i0,j0)]+t1*source[IX(i0,j1)])+
								s1*(t0*source[IX(i1,j0)]+t1*source[IX(i1,j1)]);

			//printf("%f: ",result[IX(i,j)]);

		}
	}

	//Set Boundary Condition Here
}



void MacGrid2D::advectVector(VectorField &dest,VectorField &src)
{


	DATA x,y, s0,t0,s1,t1;
	int i0,j0,i1,j1;
	DATA t = this->m_Dt*this->m_N;
	for(int i=0;i<this->m_N+3;i++)
	{
		for(int j=0;j<this->m_N+3;j++)
		{
			x = i- t* (src[IXX(i,j)].x+src[IXX(i+1,j)].y)*0.5f;
			y = j- t* (src[IXX(i,j)].x+src[IXX(i,j+1)].y)*0.5f;

			//Set boundary
			if(x<0.5f) x=0.5f;
			if(y<0.5f) y=0.5f;
			if(x>m_N+0.5f) x=m_N+0.5f;
			if(y>m_N+0.5f) y=m_N+0.5f;

			i0 =(int)x;
			j0 =(int)y;
			i1 = i0+1;
			j1 = j0+1;

			s1 = x-i0; s0 = 1-s1; t1 = y-j0; t0 = 1-t1;
			dest[IX(i,j)] = s0*(t0*src[IX(i0,j0)]+t1*src[IX(i0,j1)])+
								s1*(t0*src[IX(i1,j0)]+t1*src[IX(i1,j1)]);

		}
	}


	//DATA x,y, s0,t0,s1,t1;
	//int i0,j0,i1,j1;
	//DATA t = this->m_Dt;

	//DATA *u, *v;
	//u =  (DATA*) malloc ((m_N+3)*(m_N+3)*sizeof(DATA));
	//v =  (DATA*) malloc ((m_N+3)*(m_N+3)*sizeof(DATA));
	//
	//float a=m_Dt*m_Viscosity*m_N*m_N;

	//for(int i=0;i<(m_N+3)*(m_N+3);i++)
	//{
	//	u[i] = src[i].x;
	//	v[i] = src[i].y;
	//}
	//
	//for(int i=1;i<this->m_N+2;i++)
	//{
	//	for(int j=1;j<this->m_N+2;j++)
	//	{
	//		
	//		//x = i- t* (src[IXX(i,j)].x+src[IXX(i+1,j)].x)*0.5f;
	//		//y = j- t* (src[IXX(i,j)].y+src[IXX(i,j+1)].y)*0.5f;
	//		x = (i)- t* (src[IXX(i,j)].x);
	//		y = (j) -t* (src[IXX(i,j)].y);

	//		//Set boundary
	//		if(x<0.5f) x=0.5f;
	//		if(y<0.5f) y=0.5f;
	//		if(x>m_N+0.5f) x=m_N+0.5f;
	//		if(y>m_N+0.5f) y=m_N+0.5f;

	//		
	//		/*i0 = x-(int)x-0.5>=0? int(x)+1:int(x);
	//		j0 = y-(int)y-0.5>=0? int(y)+1:int(y);

	//		
	//		s1 = x-i0; s0 = 1-s1; t1 = y-j0; t0 = 1-t1;
	//		*/



	//		Vector<DATA,2> result = make_vector<DATA,2>(0);
	//		this->InterpolateVel(u,v,x,y,result);

	//		u[IXX(i,j)] =  result.x;
	//		
	//		v[IXX(i,j)] =  result.y;
	//		
	//	}
	//}

	//for(int i=0;i<(m_N+3)*(m_N+3);i++)
	//{
	//	dest[i].x=u[i];
	//	dest[i].y=v[i];
	//}

	//delete[] u,v;
	////Set Boundary Condition Here
}

void MacGrid2D::InterpolateVel(DATA *uField,DATA *vField, 
					DATA x, DATA y,Vector<DATA,2> &result)
{
	int i,j,i0,j0;
	//curCube ij
	i = x-(int)x-0.5>=0? int(x)+1:int(x);
	j = y-(int)y-0.5>=0? int(y)+1:int(y);
	
	i0 = int(x);
	j0 = int(y);

	DATA y0,y1,y2,y3;
	DATA x0,x1,x2,x3;

	DATA resultX, resultY;
	DATA s0,s1,t0,t1;
	
	//X
		x0 =  uField[IXX(i0,j)];
		x1 =  uField[IXX(i+1,j0)];
		x2 =  uField[IXX(i,j0+1)];
		x3 =  uField[IXX(i+1,j0+1)];

		s0 = x-(i-1+0.5);
		t0 = y-(j0);
		s1 = 1-s0;
		t1 = 1-t0;

		result.x =  t0*(s1*x2+s0*x3)+t1*(s1*x0+s0*x1);


	//Y
		y0 =  vField[IXX(i0,j)];
		y1 =  vField[IXX(i0+1,j)];
		y2 =  vField[IXX(i0,j+1)];
		y3 =  vField[IXX(i0+1,j+1)];

		s0 = x-(i0);
		t0 = y-(j-1+0.5);
		s1 = 1-s0;
		t1 = 1-t0;

		result.y =  t0*(s1*y2+y0*x3)+t1*(s1*y0+s0*y1);


}


void MacGrid2D::AddScalarField(ScalarField &a_SrcField, 
								ScalarField &destination)
{
	for(int i=0; i<((m_N+2)*(m_N+2)); i++) {
		destination[i] += (a_SrcField[i]);
		
	}
}

//void MacGrid2D::AddVectorField(ScalarField *u_SrcField,ScalarField *v_SrcField, 
//		ScalarField *u_dest,ScalarField *v_dest)
//{
//	for(int i=0; i<((m_N+3)*(m_N+3)); i++) {
//		u_dest->m_Field[i] += m_Dt*((*u_SrcField)[i]);
//		v_dest->m_Field[i] += m_Dt*((*v_SrcField)[i]);
//	}
//}

void MacGrid2D::AddVectorField(VectorField &src,VectorField &dest)
{
	for(int i=0; i<((m_N+3)*(m_N+3)); i++) {
		dest[i] += m_Dt*src[i];
	}
}

void MacGrid2D::Swap(ScalarField &src,ScalarField &dest)
{

	for(int i=0; i<((m_N+2)*(m_N+2)); i++) {
		DATA tmp = 0;
		tmp = src[i];
		src[i] = dest[i];
		dest[i] =tmp;
	}

}
void MacGrid2D::Swap(VectorField &src,VectorField &dest)
{

	for(int i=0; i<((m_N+3)*(m_N+3)); i++) {
		Vector<DATA,2> tmp = make_vector<DATA,2>(0);
		tmp = src[i];
		src[i] = dest[i];
		dest[i] =tmp;
	}

}

void MacGrid2D::DensityTimeStep(ScalarField &source, ScalarField &result,VectorField &field)
{
	
	this->AddScalarField(source,result);

	//for(int i=0;i<(m_N+2)*(m_N+2);i++)
		//printf("%f    ",source[i]);



	Swap(result,source);
	this->advectScalar(result,source,field);
}

void MacGrid2D::VelTimeStep(VectorField &src,VectorField &dest)
{
	/*
	printf("\nPrevVelSourceX:\n");
	for(int j=0;j<m_N+3;j++)
	{
		for(int i=0;i<m_N+3;i++)
		{
			printf("%f\t", dest[IXX(i,j)].x);
		}
		printf("\n");
	}
	printf("\nPrevVelSourceY:\n");
	for(int j=0;j<m_N+3;j++)
	{
		for(int i=0;i<m_N+3;i++)
		{
			printf("%f\t", dest[IXX(i,j)].y);
		}
		printf("\n");
	}*/

	
	AddVectorField(src,dest);

	//Swap(src,dest);

	//this->DiffuseVector(dest,src);
	//set_VelBnd(dest);
	
	
 	project(dest);
	
	
	Swap(src,dest);

	advectVector(dest,src);
	project(dest);
	//set_VelBnd(dest);
	
}

void MacGrid2D::project(VectorField &dest)//(ScalarField * u, ScalarField * v)
{

	
	//double *div,*p;
	double *divA,*pA;

	divA =  (double*) malloc ((m_N+2)*(m_N+2)*sizeof(double));
	pA =  (double*) malloc ((m_N+2)*(m_N+2)*sizeof(double));
	
	for(int j=0;j<m_N+2;j++)
	{
		for(int i=0;i<m_N+2;i++)
		{
			divA[IX(i,j)]=pA[IX(i,j)]=0;
		}
	}

	/*printf("\ncurX:\n");
	for(int j=0;j<m_N+3;j++)
	{
		for(int i=0;i<m_N+3;i++)
		{
			printf("%f\t", dest[IXX(i,j)].x);
		}
		printf("\n");
	}
	printf("\ncurY:\n");
	for(int j=0;j<m_N+3;j++)
	{
		for(int i=0;i<m_N+3;i++)
		{
			printf("%f\t", dest[IXX(i,j)].y);
		}
		printf("\n");
	}
	*/
	for(int j=0;j<m_N+2;j++)
	{
		for(int i=0;i<m_N+2;i++)
		{
			divA[IX(i,j)] = //1.0f;
				( dest[IXX(i+1,j)].x -dest[IXX(i,j)].x +
				dest[IXX(i,j+1)].y-dest[IXX(i,j)].y );
			pA[IX(i,j)] = 0;
		}
	}
	//printf("\nDivergence1,1:\n");
	//printf("\nDiv:\tdest[IXX(2,1)].x-dest[IXX(1,1)].x+dest[IXX(1,2)].y-dest[IXX(1,1)].y");
	//
	//printf("\nDiv:%f, %f-%f+%f-%f\n",divA[IXX(1,1)],
	//	dest[IXX(2,1)].x,dest[IXX(1,1)].x ,
	//			dest[IXX(1,2)].y,dest[IXX(1,1)].y);
	////this->set_ScarBnd(divA);
	//
	
	int steps = 50;
	projectMatrix *a = new projectMatrix(this->m_N);
	/*printf("\nDivergenceRightSide:\n");
	for(int j=0;j<m_N+2;j++)
	{

		for(int i=0;i<m_N+2;i++)
		{
			divA[IX(i,j)]=0.0f;
			printf("%f\t", divA[IX(i,j)]);

		}
		printf("\n");
	}
	divA[IX(0,0)]=1.0f;
	*/
	double err = ConjGrad((m_N+2)*(m_N+2),a,pA,divA,1.0e-2,&steps);
/*
	printf("\nErr:%f Pressure:\n",err);
	for(int j=0;j<m_N+2;j++)
	{
		for(int i=0;i<m_N+2;i++)
		{
			printf("%f\t", pA[IX(i,j)]);
		}
		printf("\n");
	}*/


	for(int i=1;i<m_N+2;i++)
	{
		for(int j=1;j<m_N+2;j++)
		{
			//printf("???%f:%f,",dest[IXX(i,j)].x ,dest[IXX(i,j)].y );
			dest[IXX(i,j)].x -= (pA[IX(i,j)]-pA[IX(i-1,j)]);
			dest[IXX(i,j)].y -= (pA[IX(i,j)]-pA[IX(i,j-1)]);
			
		}
	}
	//this->set_VelBnd(dest);
	delete[] divA,pA;
	delete a;
/*
	printf("\n\nXField:\n");
	for(int j=0;j<m_N+3;j++)
	{
		printf("\n");
		for(int i=0;i<m_N+3;i++)
		{
			printf("%f\t",dest[IXX(i,j)].x);
		}
	}

	printf("\n\nYField:\n");
	for(int j=0;j<m_N+3;j++)
	{
		printf("\n");
		for(int i=0;i<m_N+3;i++)
		{
			printf("%f\t",dest[IXX(i,j)].y);
		}
	}
	

	*/
	/*printf("\n\nDivAfter:\n");
	for(int j=0;j<m_N+2;j++)
	{
		printf("\n");
		for(int i=0;i<m_N+2;i++)
		{
			printf("%f\t",dest[IXX(i+1,j)].x-dest[IXX(i,j)].x+
				dest[IXX(i,j+1)].y-dest[IXX(i,j+1)].y);
		}
	}*/


}

void MacGrid2D::lin_solve ( float * x, float * x0, float a, float c )
{
	int i, j, k;

	for ( k=0 ; k<20 ; k++ ) {
		for ( i=1 ; i<=m_N ; i++ ) { for ( j=1 ; j<=m_N ; j++ ) {
			x[IX(i,j)] = (x0[IX(i,j)] + a*(x[IX(i-1,j)]+x[IX(i+1,j)]+x[IX(i,j-1)]+x[IX(i,j+1)]))/c;
		}}
	}
}

void MacGrid2D::diffuse (float * x, float * x0)
{
	float a=m_Dt*m_Viscosity*m_N*m_N;
	lin_solve ( x, x0, a, 1+4*a );
}


void MacGrid2D::DiffuseVector (VectorField &result, VectorField &src)
{

	DATA *px,*py, *x, *y;
	px =  (DATA*) malloc ((m_N+3)*(m_N+3)*sizeof(DATA));
	py =  (DATA*) malloc ((m_N+3)*(m_N+3)*sizeof(DATA));
	x = (DATA*) malloc ((m_N+3)*(m_N+3)*sizeof(DATA));
	y = (DATA*) malloc ((m_N+3)*(m_N+3)*sizeof(DATA));
	
	float a=m_Dt*m_Viscosity*m_N*m_N;

	for(int i=0;i<(m_N+3)*(m_N+3);i++)
	{
		px[i] = src[i].x;
		py[i] = src[i].y;
	}
//	lin_solve ( x, px, a, 1+4*a );
//	lin_solve ( y, py, a, 1+4*a );
	for(int i=0;i<(m_N+3)*(m_N+3);i++)
	{
		result[i].x = x[i];
		result[i].y = y[i];
	
	}
	delete[] px,py,x,y;
}
//
//void MacGrid2D::set_VelBnd (ScalarField * u,ScalarField * v )
//{
//	int i;
//
//	for ( i=0 ; i<= this->m_N+1; i++ ) {
//		
//		u[IXX(0,i)]=u[IXX(1,i)]=0;
//		u[IXX(m_N,i)]= u[IXX(m_N+1,i)]=0;
//
//		v[IXX(i,0)]=v[IXX(i,1)]=0;
//		v[IXX(i,m_N)]=v[IXX(i,m_N+1)]=0;
//	}
//}

void MacGrid2D::set_VelBnd (VectorField &field)
{
	for (int i=1 ; i<= this->m_N+1; i++ ) {

		field[IXX(0,i)].x =field[IXX(1,i)].x = 0;
		field[IXX(m_N+1,i)].x= field[IXX(m_N+2,i)].x=0;
		
		field[IXX(i,0)].y=field[IXX(i,1)].y=0;
		field[IXX(i,m_N+1)].y=field[IXX(i,m_N+2)].y=0;
	}
	field[IXX(0,0)].x = 0.5f*(field[IXX(1,0)].x + field[IXX(0,1)].x);
	field[IXX(0,m_N+2)].x = 0.5f*(field[IXX(1,m_N+2)].x + field[IXX(0,m_N+1)].x);
	field[IXX(m_N+2,0)].x = 0.5f*(field[IXX(m_N+1,0)].x + field[IXX(m_N+2,1)].x);
	field[IXX(m_N+2,m_N+2)].x = 0.5f*(field[IXX(m_N+1,m_N+2)].x + field[IXX(m_N+2,m_N+1)].x);

}
void MacGrid2D::set_ScarBnd (DATA * x)
{
	for (int i=1 ; i< this->m_N+1; i++ ) {
		x[IX(0,i)]=x[IX(1,i)];
		x[IX(m_N+1,i)]=x[IX(m_N,i)];
		
		x[IX(i,0)]=x[IX(i,1)];
		x[IX(i,m_N+1)]= x[IX(i,m_N)];
	}
	
	x[IX(0  ,0  )] = 0.5f*((x[IX(1,0  )]) + x[IX(0  ,1)]);
	x[IX(0  ,m_N+1)] = 0.5f*(x[IX(1,m_N+1)] + x[IX(0  ,m_N)]);
	x[IX(m_N+1,0  )] = 0.5f*(x[IX(m_N,0  )] + x[IX(m_N+1,1)]);
	x[IX(m_N+1,m_N+1)] = 0.5f*(x[IX(m_N,m_N+1)]+x[IX(m_N+1,m_N)]);
}

void MacGrid2D::draw()
{

	int i, j;
	float x, y, h;

	h = 1.0f/this->m_N;

	glColor3f ( 1.0f, 1.0f, 1.0f );
	glLineWidth ( 1.0f );

	glBegin ( GL_LINES );

	for ( i=0 ; i<m_N+2; i++ ) {
		x = (i)*h;
		for ( j=0 ; j<m_N+2; j++ ) {
			y = (j)*h;
			
			glVertex2f ( x, y );
			glVertex2f (x+ h,y);
			glVertex2f ( x, y );
			glVertex2f (x,y+h);
		}
	}
	glEnd();

}