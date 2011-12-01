#include <cloth/cloth.hpp>

#include <cstdlib>
//static std::vector<particle*> pvector;
//static std::vector<force*> fvector;
//static std::vector<constrain*> cvector;
//static std::vector<integrater*> integratervector;
//static float dt;

int NumOfParti = -1;

Cloth::Cloth(std::vector<Particle*> &pvector,std::vector<Force*> &fvector){
	//this->pvector = *parti;
	//this->fvector = *forces;
	DRAG=0.2;
	Damp=15;
	KS=50;
	//WindStrength = 50.0f;
	//WindDir =  make_vector(0.0f,0.3f, 0.5f);
	DATA dist = 0.4;
	DATA Mass = 1;
	DATA R =0.01;
	Vector<DATA,3> center = make_vector(1.0, 1.5, 0.0);
	U=24;
	V=24;
	Vector<DATA,3> uoffset=make_vector(dist, 0.0, 0.0);
	Vector<DATA,3> voffset=make_vector(0.0, -dist, 0.0);
	Vector<DATA,3> originalpos= center-((int)U/2)*uoffset-((int)V/2)*voffset;
	
	for(int row=0;row<U;row++)
	{
		
		for(int col=0;col<V;col++)
		{
			int curindex= col+row*U;

			Vector<DATA,3> partpos = originalpos + row*voffset+col*uoffset;
			pvector.push_back(new Particle(++NumOfParti,partpos,Mass,10,10, R));
		
		}
	}
	//apply wind force
	for(int row=0;row<U-1;row++)
	{
		
		for(int col=0;col<V-1;col++)
		{
			int curindex= col+row*U;

			/*fvector.push_back(
				new windforce(pvector[curindex],pvector[curindex+1],pvector[curindex+U+1],pvector[curindex+U],&windstrength,&winddir)
				);*/
			
		}
	}

	//row force
	for(int row=0;row<U;row++)
	{
		for(int col=0;col<V-1;col++)
		{
			int curindex= col+row*U;
			double restlength=length(uoffset);
			fvector.push_back(new SpringForce(pvector[curindex], pvector[curindex+1], restlength, this->KS, this->Damp));
			
		}
	}
	//col force
	for(int row=0;row<U-1;row++)
	{
		for(int col=0;col<V;col++)
		{
			int curindex= col+row*U;
			double restlength=length(voffset);
			fvector.push_back(new SpringForce(pvector[curindex], pvector[curindex+U], restlength, this->KS, this->Damp));
			
		}
	}

	//forward cross force
	for(int row=0;row<U-1;row++)
	{
		for(int col=0;col<V-1;col++)
		{
			int curindex= col+row*U;
			double restlength=length(voffset+uoffset);
			fvector.push_back(new SpringForce(pvector[curindex], pvector[curindex+U+1], restlength, this->KS, this->Damp));
		
		}
	}

	//backward cross force
	for(int row=0;row<U-1;row++)
	{
		for(int col=1;col<V;col++)
		{
			int curindex= col+row*U;
			double restlength=length(voffset+uoffset);
			fvector.push_back(new SpringForce(pvector[curindex], pvector[curindex+U-1], restlength, this->KS, this->Damp));
			
		}
	}

	//horizontal cross force
	for(int row=0;row<U;row++)
	{
		for(int col=0;col<V-2;col++)
		{
			int curindex= col+row*U;
			double restlength=length(2*uoffset);
			fvector.push_back(new SpringForce(pvector[curindex], pvector[curindex+2], restlength, this->KS, this->Damp));
			
			
		}
	}

	//verticle cross force
	for(int row=0;row<U-2;row++)
	{
		for(int col=0;col<V;col++)
		{
			int curindex= col+row*U;
			double restlength=length(2*voffset);
			fvector.push_back(new SpringForce(pvector[curindex], pvector[curindex+U+U], restlength, this->KS, this->Damp));
		
		}
	}



}

Cloth::~Cloth(){
	//pvector.clear();
	//fvector.clear();
}

void Cloth::reset(){
	/*int size = pvector.size();
	for(int ii=0; ii<size; ii++){
		pvector[ii]->reset();
		
	}*/
}

void Cloth::draw(std::vector<Particle*> &pVector,std::vector<Force*> &fVector){
	
	int size = fVector.size();
	for(int ii=0; ii<size; ii++){
		fVector[ii]->draw();
	}
	int color=0;
	int baseCol;
	int tmp = V%2? 1.0f:0.0f;
	
	for(int row=0;row<U-1;row++)
	{
		
		for(int col=0;col<V-1;col++)
		{
			baseCol = row%2?0:1;
			int curIndex= col+row*U;

			if(col%2==0)
				color=baseCol;
			else if(col%2!=0)
				color=!baseCol;
			//col= curIndex%2? 0.0f:1.0f;
			//if(curIndex!=0&&curIndex+1%U==0)
			
			glColor3f(color,color,color);
			glBegin(GL_QUADS);
			glVertex3f(pVector[curIndex]->m_Position.x,pVector[curIndex]->m_Position.y,pVector[curIndex]->m_Position.z);
			glVertex3f(pVector[curIndex+U]->m_Position.x,pVector[curIndex+U]->m_Position.y,pVector[curIndex+U]->m_Position.z);
			glVertex3f(pVector[curIndex+U+1]->m_Position.x,pVector[curIndex+U+1]->m_Position.y,pVector[curIndex+U+1]->m_Position.z);
			glVertex3f(pVector[curIndex+1]->m_Position.x,pVector[curIndex+1]->m_Position.y,pVector[curIndex+1]->m_Position.z);

			glEnd();
		}
	}
	
}
