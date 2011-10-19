#include <cloth/cloth.hpp>

#include <cstdlib>
//static std::vector<particle*> pvector;
//static std::vector<force*> fvector;
//static std::vector<constrain*> cvector;
//static std::vector<integrater*> integratervector;
static float dt;



Cloth::Cloth(){

	DRAG=0.2f;
	DAMP=4.0f;
	KS=10.0f;
	WindStrength = 50.0f;
	WindDir =  make_vector(0.0f,0.3f, 0.5f);

	Vector3f center = make_vector(0.0f, 50.0f, 0.0f);
	U=15;
	V=15;
	Vector3f uoffset=make_vector(1.0f, 0.0f, 0.0f);
	Vector3f voffset=make_vector(0.0f, -1.0f, 0.0f);
	Vector3f originalpos= center-((int)U/2)*uoffset-((int)V/2)*voffset;
	ClothGen(U,V,uoffset,voffset,originalpos);
	
	//cvector.push_back(new pointconstrain(pvector[0]));
	//cvector.push_back(new pointconstrain(pvector[v-1]));

	//integratervector.push_back(new eulerintegrater(this));
	//integratervector.push_back(new verletintegrater(this));

}

void Cloth::ClothGen(int u, int v, Vector3f uoffset, Vector3f voffset, Vector3f originalpos)
{
	for(int row=0;row<u;row++)
	{
		
		for(int col=0;col<v;col++)
		{
			int curindex= col+row*u;

			Vector3f partpos = originalpos + row*voffset+col*uoffset;
			//pvector.push_back(new particle(partpos));
			//fvector.push_back(new gravityforce(pvector[curindex]));
			
		}
	}
	//apply wind force
	for(int row=0;row<u-1;row++)
	{
		
		for(int col=0;col<v-1;col++)
		{
			int curindex= col+row*u;

			/*fvector.push_back(
				new windforce(pvector[curindex],pvector[curindex+1],pvector[curindex+u+1],pvector[curindex+u],&windstrength,&winddir)
				);*/
			
		}
	}

	//row force
	for(int row=0;row<u;row++)
	{
		for(int col=0;col<v-1;col++)
		{
			int curindex= col+row*u;
			double restlength=length_squared(uoffset);
			/*fvector.push_back(
				new springforce(pvector[curindex], pvector[curindex+1], restlength,&this->ks,&this->damp));
			*/
		}
	}
	//col force
	for(int row=0;row<u-1;row++)
	{
		for(int col=0;col<v;col++)
		{
			int curindex= col+row*u;
			double restlength=length_squared(voffset);
			/*fvector.push_back(
				new springforce(pvector[curindex], pvector[curindex+u], restlength,&this->ks,&this->damp));
			*/
		}
	}

	//forward cross force
	for(int row=0;row<u-1;row++)
	{
		for(int col=0;col<v-1;col++)
		{
			int curindex= col+row*u;
			double restlength=length_squared(voffset+uoffset);

			/*fvector.push_back(
				new springforce(pvector[curindex], pvector[curindex+u+1], restlength,&this->ks,&this->damp));
			*/
		}
	}

	//backward cross force
	for(int row=0;row<u-1;row++)
	{
		for(int col=1;col<v;col++)
		{
			int curindex= col+row*u;
			double restlength=length_squared(voffset+uoffset);

			/*fvector.push_back(
				new springforce(pvector[curindex], pvector[curindex+u-1], restlength,&this->ks,&this->damp));
			*/
		}
	}

	//horizontal cross force
	for(int row=0;row<u;row++)
	{
		for(int col=0;col<v-2;col++)
		{
			int curindex= col+row*u;
			double restlength=length_squared(2*uoffset);

			/*fvector.push_back(
				new springforce(pvector[curindex], pvector[curindex+2], restlength,&this->ks,&this->damp));
			*/
		}
	}

	//verticle cross force
	for(int row=0;row<u-2;row++)
	{
		for(int col=0;col<v;col++)
		{
			int curindex= col+row*u;
			double restlength=length_squared(2*voffset);

			/*fvector.push_back(
				new springforce(pvector[curindex], pvector[curindex+u+u], restlength,&this->ks,&this->damp));
			*/
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

void Cloth::draw(){
	/*int size = pvector.size();
	
	for(int ii=0; ii<size; ii++){
		pvector[ii]->draw();
	}
	
	*/
	
	//int size = fvector.size();
	//for(int ii=0; ii<size; ii++){
	//	fvector[ii]->draw();
	//}
	//int color=0;
	//int basecol;
	//int tmp = v%2? 1.0f:0.0f;
	//
	//for(int row=0;row<u-1;row++)
	//{
	//	
	//	for(int col=0;col<v-1;col++)
	//	{
	//		basecol = row%2?0:1;
	//		int curindex= col+row*u;

	//		if(col%2==0)
	//			color=basecol;
	//		else if(col%2!=0)
	//			color=!basecol;
	//		//col= curindex%2? 0.0f:1.0f;
	//		//if(curindex!=0&&curindex+1%u==0)
	//		
	//		glcolor3f(color,color,color);
	//		glbegin(gl_quads);
	//		glvertex3f(pvector[curindex]->m_position.x,pvector[curindex]->m_position.y,pvector[curindex]->m_position.z);
	//		glvertex3f(pvector[curindex+u]->m_position.x,pvector[curindex+u]->m_position.y,pvector[curindex+u]->m_position.z);
	//		glvertex3f(pvector[curindex+u+1]->m_position.x,pvector[curindex+u+1]->m_position.y,pvector[curindex+u+1]->m_position.z);
	//		glvertex3f(pvector[curindex+1]->m_position.x,pvector[curindex+1]->m_position.y,pvector[curindex+1]->m_position.z);

	//		glend();
	//	}
	//}
}

void Cloth::simulation_step(float elaspedtime)
{

	
	//for(int i=0; i<pvector.size();i++)
	//{
	//	pvector[i]->clearforce();
	//}

	////apply the force
	//for(int i=0; i<fvector.size();i++)
	//{
	//	fvector[i]->apply_force();
	//}

	////integrate
	//for(int i=0; i<pvector.size();i++)
	//{
	//	vector3f acc= pvector[i]->m_forceaccumulator/pvector[i]->m_mass;
	//	integratervector[integrater_index]->integrate(acc,pvector[i],elaspedtime);
	//}


	////constrain
	//for(int i=0; i<cvector.size();i++)
	//{
	//	cvector[i]->apply_constrain();
	//}

	
}