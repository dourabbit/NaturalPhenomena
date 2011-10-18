//#include <Particle\Particle.h>
//
//#include <vector>
//#include <Integrators\Integrator.hpp>

//#include <Integrators\common.h>
#include <Integrators\Solver.hpp>
Solver::Solver(){
	

}

Solver::~Solver(){
	delete _constSolver;
	delete[] this->PhaseSpace;
	delete[] this->PhaseSpaceDot;
}

void Solver::update( std::vector<Particle*> pVector, DATA dt, 
					Integrator* integrator)
{
	//Test
	///*int ii, size = pVector.size();
	//
	//for(ii=0; ii<size; ii++)
	//{
	//	pVector[ii]->m_Position += dt*pVector[ii]->m_Velocity;
	//	pVector[ii]->m_Velocity = DAMP*pVector[ii]->m_Velocity + make_vector< float >(RAND,RAND,RAND) * 0.005f;
	//}*/

	for(int ii=0; ii<pVector.size(); ii++)
	{
		pVector[ii]->m_ForceAccumulator = make_vector(0.0f,0.0f,0.0f);
	}


	for(int ii=0; ii<this->_pForces.size();ii++)
	{
		_pForces[ii]->apply_force();
	}

	this->_constSolver->Solve();


	getDerivative();
	integrator->Integrate(pVector, dt);
	//pVector[0]->m_Position = make_vector(0.0,0.0,0.0);
}

void Solver::Initialize(const int &numOfParti, std::vector<Particle*> pParti,std::vector<Force*> pForces, std::vector<Constraint*> pConstraints)
{
	
	this->_numOfParti= numOfParti;
	this->_pParti = pParti;
	this->_pForces = pForces;
	this->_pConstraints = pConstraints;
	
	
	this->_constSolver = new ConstraintSolver(this);
	
	this->PhaseSpace = new DATA[_numOfParti*6];
	this->PhaseSpaceDot = new DATA[_numOfParti*6];
	///*this->_px = new DATA[_numOfParti*3];
	//this->_pDx = new DATA[_numOfParti*3];
	//
	//this->_pJacobi = new DATA[_numOfParti*6*2];*/
	//


	//
	//for(int ii=0; ii<pParti.size();ii++)
	//{
	//	this->_px = &(pParti[ii]->m_Position.x);
	//	_px++;
	//	this->_px = &(pParti[ii]->m_Position.y);
	//	_px++;
	//	this->_px = &(pParti[ii]->m_Position.z);
	//	_px++;
	//	this->_px = &(pParti[ii]->m_Velocity.x);
	//	_px++;
	//	this->_px = &(pParti[ii]->m_Velocity.y);
	//	_px++;
	//	this->_px = &(pParti[ii]->m_Velocity.z);
	//}


}

void Solver::getDerivative()
{

	///*for(int ii=0; ii<this->_pParti.size();ii++)
	//{

	//	this->_pDx = &(_pParti[ii]->m_Velocity.x);
	//	_pDx++;
	//	this->_pDx = &(_pParti[ii]->m_Velocity.y);
	//	_pDx++;
	//	this->_pDx = &(_pParti[ii]->m_Velocity.z);
	//	_pDx++;

	//	Vector<DATA,3> acc = (_pParti[ii]->m_ForceAccumulator/_pParti[ii]->m_Mass);

	//	this->_pDx = &(acc.x);
	//	_pDx++;
	//	this->_pDx = &(acc.y);
	//	_pDx++;
	//	this->_pDx = &(acc.z);
	//}*/

	for(int ii=0; ii<this->_pParti.size();ii++)
	{
		this->PhaseSpace[ii] = _pParti[ii]->m_Position.x;
		this->PhaseSpace[ii+1] = _pParti[ii]->m_Position.y;
		this->PhaseSpace[ii+2] = _pParti[ii]->m_Position.z;

		this->PhaseSpace[ii+3] = _pParti[ii]->m_Velocity.x;
		this->PhaseSpace[ii+4] = _pParti[ii]->m_Velocity.y;
		this->PhaseSpace[ii+5] = _pParti[ii]->m_Velocity.z;
	}


	for(int ii=0; ii<this->_pParti.size();ii++)
	{
		this->PhaseSpaceDot[ii] = _pParti[ii]->m_Velocity.x;
		this->PhaseSpaceDot[ii+1] = _pParti[ii]->m_Velocity.y;
		this->PhaseSpaceDot[ii+2] = _pParti[ii]->m_Velocity.z;

		Vector<DATA,3> acc = (_pParti[ii]->m_ForceAccumulator/_pParti[ii]->m_Mass);
		this->PhaseSpaceDot[ii+3] = acc.x;
		this->PhaseSpaceDot[ii+4] = acc.y;
		this->PhaseSpaceDot[ii+5] = acc.z;
	}

}


