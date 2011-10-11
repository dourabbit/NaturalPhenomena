//#include <Cloth/Cloth.hpp>
//#include <Particle/tmp/SpringForce.hpp>
//#include <Particle/tmp/Constrain.hpp>
//#include <Particle/tmp/WindForce.hpp>
//#include <Integrators/Integrater.hpp>
//#include <Integrators/EulerIntegrater.hpp>
//#include <Integrators/VerletIntegrater.hpp>
//#include <cstdlib>
//static std::vector<Particle*> pVector;
//static std::vector<Force*> fVector;
//static std::vector<Constrain*> cVector;
//static std::vector<Integrater*> IntegraterVector;
//static float dt;
//
//
//
//Cloth::Cloth(){
//
//	INTEGRATER_INDEX=0;
//	DRAG=0.2f;
//	DAMP=4.0f;
//	KS=10.0f;
//	WindStrength = 50.0f;
//	WindDir =  make_vector(0.0f,0.3f, 0.5f);
//
//	Vector3f center = make_vector(0.0f, 50.0f, 0.0f);
//	U=15;
//	V=15;
//	Vector3f uOffset=make_vector(1.0f, 0.0f, 0.0f);
//	Vector3f vOffset=make_vector(0.0f, -1.0f, 0.0f);
//	Vector3f originalPos= center-((int)U/2)*uOffset-((int)V/2)*vOffset;
//	ClothGen(U,V,uOffset,vOffset,originalPos);
//	
//	cVector.push_back(new PointConstrain(pVector[0]));
//	cVector.push_back(new PointConstrain(pVector[V-1]));
//
//	IntegraterVector.push_back(new EulerIntegrater(this));
//	IntegraterVector.push_back(new VerletIntegrater(this));
//
//}
//
//void Cloth::ClothGen(int U, int V, Vector3f UOffset, Vector3f VOffset, Vector3f originalPos)
//{
//	for(int row=0;row<U;row++)
//	{
//		
//		for(int col=0;col<V;col++)
//		{
//			int curIndex= col+row*U;
//
//			Vector3f partPos = originalPos + row*VOffset+col*UOffset;
//			pVector.push_back(new Particle(partPos));
//			fVector.push_back(new GravityForce(pVector[curIndex]));
//			
//		}
//	}
//	//Apply wind force
//	for(int row=0;row<U-1;row++)
//	{
//		
//		for(int col=0;col<V-1;col++)
//		{
//			int curIndex= col+row*U;
//
//			fVector.push_back(
//				new WindForce(pVector[curIndex],pVector[curIndex+1],pVector[curIndex+U+1],pVector[curIndex+U],&WindStrength,&WindDir)
//				);
//			
//		}
//	}
//
//	//row force
//	for(int row=0;row<U;row++)
//	{
//		for(int col=0;col<V-1;col++)
//		{
//			int curIndex= col+row*U;
//			double restLength=length_squared(UOffset);
//			fVector.push_back(
//				new SpringForce(pVector[curIndex], pVector[curIndex+1], restLength,&this->KS,&this->DAMP));
//			
//		}
//	}
//	//col force
//	for(int row=0;row<U-1;row++)
//	{
//		for(int col=0;col<V;col++)
//		{
//			int curIndex= col+row*U;
//			double restLength=length_squared(VOffset);
//			fVector.push_back(
//				new SpringForce(pVector[curIndex], pVector[curIndex+U], restLength,&this->KS,&this->DAMP));
//			
//		}
//	}
//
//	//forward cross force
//	for(int row=0;row<U-1;row++)
//	{
//		for(int col=0;col<V-1;col++)
//		{
//			int curIndex= col+row*U;
//			double restLength=length_squared(VOffset+UOffset);
//
//			fVector.push_back(
//				new SpringForce(pVector[curIndex], pVector[curIndex+U+1], restLength,&this->KS,&this->DAMP));
//			
//		}
//	}
//
//	//backward cross force
//	for(int row=0;row<U-1;row++)
//	{
//		for(int col=1;col<V;col++)
//		{
//			int curIndex= col+row*U;
//			double restLength=length_squared(VOffset+UOffset);
//
//			fVector.push_back(
//				new SpringForce(pVector[curIndex], pVector[curIndex+U-1], restLength,&this->KS,&this->DAMP));
//			
//		}
//	}
//
//	//horizontal cross force
//	for(int row=0;row<U;row++)
//	{
//		for(int col=0;col<V-2;col++)
//		{
//			int curIndex= col+row*U;
//			double restLength=length_squared(2*UOffset);
//
//			fVector.push_back(
//				new SpringForce(pVector[curIndex], pVector[curIndex+2], restLength,&this->KS,&this->DAMP));
//			
//		}
//	}
//
//	//verticle cross force
//	for(int row=0;row<U-2;row++)
//	{
//		for(int col=0;col<V;col++)
//		{
//			int curIndex= col+row*U;
//			double restLength=length_squared(2*VOffset);
//
//			fVector.push_back(
//				new SpringForce(pVector[curIndex], pVector[curIndex+U+U], restLength,&this->KS,&this->DAMP));
//			
//		}
//	}
//
//
//
//}
//
//Cloth::~Cloth(){
//	pVector.clear();
//	fVector.clear();
//}
//
//void Cloth::reset(){
//	int size = pVector.size();
//	for(int ii=0; ii<size; ii++){
//		pVector[ii]->reset();
//		
//	}
//}
//
//void Cloth::draw(){
//	/*int size = pVector.size();
//	
//	for(int ii=0; ii<size; ii++){
//		pVector[ii]->draw();
//	}
//	
//	*/
//	
//	int size = fVector.size();
//	for(int ii=0; ii<size; ii++){
//		fVector[ii]->draw();
//	}
//	int color=0;
//	int baseCol;
//	int tmp = V%2? 1.0f:0.0f;
//	
//	for(int row=0;row<U-1;row++)
//	{
//		
//		for(int col=0;col<V-1;col++)
//		{
//			baseCol = row%2?0:1;
//			int curIndex= col+row*U;
//
//			if(col%2==0)
//				color=baseCol;
//			else if(col%2!=0)
//				color=!baseCol;
//			//col= curIndex%2? 0.0f:1.0f;
//			//if(curIndex!=0&&curIndex+1%U==0)
//			
//			glColor3f(color,color,color);
//			glBegin(GL_QUADS);
//			glVertex3f(pVector[curIndex]->m_Position.x,pVector[curIndex]->m_Position.y,pVector[curIndex]->m_Position.z);
//			glVertex3f(pVector[curIndex+U]->m_Position.x,pVector[curIndex+U]->m_Position.y,pVector[curIndex+U]->m_Position.z);
//			glVertex3f(pVector[curIndex+U+1]->m_Position.x,pVector[curIndex+U+1]->m_Position.y,pVector[curIndex+U+1]->m_Position.z);
//			glVertex3f(pVector[curIndex+1]->m_Position.x,pVector[curIndex+1]->m_Position.y,pVector[curIndex+1]->m_Position.z);
//
//			glEnd();
//		}
//	}
//}
//
//void Cloth::simulation_step(float elaspedTime)
//{
//
//	///TODO:
//	
//	///first, you need to clear force accumulators for all the particles
//
//	///second, apply forces to them
//
//	///if you want to implement hard constraints, the third step is to calculate constraint forces
//	///for the basic cloth simulation, you can skip this.
//
//	///Then, we can move forward
//	///Change this to others if you want to implement RK2 or RK4 or other integration method
//	
//	//clear
//	for(int i=0; i<pVector.size();i++)
//	{
//		pVector[i]->clearForce();
//	}
//
//	//Apply the force
//	for(int i=0; i<fVector.size();i++)
//	{
//		fVector[i]->apply_force();
//	}
//
//	//Integrate
//	for(int i=0; i<pVector.size();i++)
//	{
//		Vector3f acc= pVector[i]->m_ForceAccumulator/pVector[i]->m_Mass;
//		IntegraterVector[INTEGRATER_INDEX]->Integrate(acc,pVector[i],elaspedTime);
//	}
//
//
//	//Constrain
//	for(int i=0; i<cVector.size();i++)
//	{
//		cVector[i]->apply_Constrain();
//	}
//
//	///Finally, if you want to implement collisions, you could solve them here
//	///for the basic cloth simulation, you can skip this.
//}
//
////void Cloth::forward_euler(Vector3f acc, Particle* pParticle, float elaspedTime){
////	///TODO:
////	///Modify things below to take a real euler step
////	//int ii, size = pVector.size();
////
////	//for(ii=0; ii<size; ii++){
////	//	pVector[ii]->m_Position += dt*pVector[ii]->m_Velocity;
////	//	pVector[ii]->m_Velocity = DAMP*pVector[ii]->m_Velocity;// + make_vector< float >(RAND, RAND, 0.0)*0.005;
////	//}
////
////	pParticle->m_Position+= pParticle->m_Velocity*elaspedTime;
////	pParticle->m_Velocity+= acc * elaspedTime;
////}
////void Cloth::verlet_noVelocity(Vector3f acc, Particle* pParticle, float elaspedTime){
////
////	
////	Vector3f newPos= (2-DRAG)* pParticle->m_Position
////		-(1-DRAG)*pParticle->prevPosition
////		+acc*elaspedTime*elaspedTime;
////	pParticle->prevPosition = pParticle->m_Position;
////	pParticle->m_Position =newPos;
////
////	pParticle->m_Velocity+= acc * elaspedTime;
////
////}
//
