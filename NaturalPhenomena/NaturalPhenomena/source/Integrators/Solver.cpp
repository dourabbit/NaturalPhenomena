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
	

	for(int ii=0; ii<pVector.size(); ii++)
	{
		pVector[ii]->m_ForceAccumulator = make_vector(0.0f,0.0f,0.0f);
	}


	for(int ii=0; ii<this->_pForces.size();ii++)
	{
		_pForces[ii]->apply_force();
	}

	//this->_constSolver->Solve();


	getDerivative();
 	integrator->Integrate(dt);

	for(int ii=1; ii<pVector.size(); ii++)
	{
		pVector[ii]->m_Position.x = this->PhaseSpace[ii*6];
		pVector[ii]->m_Position.y = this->PhaseSpace[ii*6+1];
		pVector[ii]->m_Position.z = this->PhaseSpace[ii*6+2];

		pVector[ii]->m_Velocity.x = this->PhaseSpace[ii*6+3];
		pVector[ii]->m_Velocity.y = this->PhaseSpace[ii*6+4];
		pVector[ii]->m_Velocity.z = this->PhaseSpace[ii*6+5];

	}

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


}

void Solver::getDerivative()
{
	for(int ii=0; ii<this->_pParti.size();ii++)
	{
		int particleIndex = ii;
		int phaseIndex = particleIndex*6;
		this->PhaseSpace[phaseIndex]   = _pParti[particleIndex]->m_Position.x;
		this->PhaseSpace[phaseIndex+1] = _pParti[particleIndex]->m_Position.y;
		this->PhaseSpace[phaseIndex+2] = _pParti[particleIndex]->m_Position.z;

		this->PhaseSpace[phaseIndex+3] = _pParti[particleIndex]->m_Velocity.x;
		this->PhaseSpace[phaseIndex+4] = _pParti[particleIndex]->m_Velocity.y;
		this->PhaseSpace[phaseIndex+5] = _pParti[particleIndex]->m_Velocity.z;
	}


	for(int ii=0; ii<this->_pParti.size();ii++)
	{
		int particleIndex = ii;
		int phaseIndex = particleIndex*6;
		this->PhaseSpaceDot[phaseIndex]   = _pParti[particleIndex]->m_Velocity.x;
		this->PhaseSpaceDot[phaseIndex+1] = _pParti[particleIndex]->m_Velocity.y;
		this->PhaseSpaceDot[phaseIndex+2] = _pParti[particleIndex]->m_Velocity.z;

		Vector<DATA,3> acc = (_pParti[particleIndex]->m_ForceAccumulator/
							  _pParti[ii]->m_Mass);
		this->PhaseSpaceDot[phaseIndex+3] = acc.x;
		this->PhaseSpaceDot[phaseIndex+4] = acc.y;
		this->PhaseSpaceDot[phaseIndex+5] = acc.z;
	}

}


