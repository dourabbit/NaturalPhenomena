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
	/*delete[] this->PhaseSpace;
	delete[] this->PhaseSpaceDot;*/
}

void Solver::update(DATA dt, Integrator* integrator)
{
	
	if(this->DIM!= integrator->DIM)
		integrator->Initialize();

 	integrator->Integrate(dt);
	
}

void Solver::setState(DATA* phaseSpace)
{

	for(int ii=0; ii<this->_pParti.size(); ii++)
	{
		_pParti[ii]->m_Position.x = *(phaseSpace+6*ii);
		_pParti[ii]->m_Position.y = *(phaseSpace+1+6*ii);
		_pParti[ii]->m_Position.z = *(phaseSpace+2+6*ii);

		_pParti[ii]->m_Velocity.x = *(phaseSpace+3+6*ii);
		_pParti[ii]->m_Velocity.y = *(phaseSpace+4+6*ii);
		_pParti[ii]->m_Velocity.z = *(phaseSpace+5+6*ii);
	}
}

void Solver::getState(DATA* phaseSpace)
{
	for(int ii=0; ii<this->_pParti.size();ii++)
	{
		
		*(phaseSpace+6*ii)	 = _pParti[ii]->m_Position.x;
		*(phaseSpace+1+6*ii) = _pParti[ii]->m_Position.y;
		*(phaseSpace+2+6*ii) = _pParti[ii]->m_Position.z;

		*(phaseSpace+3+6*ii) = _pParti[ii]->m_Velocity.x;
		*(phaseSpace+4+6*ii) = _pParti[ii]->m_Velocity.y;
		*(phaseSpace+5+6*ii) = _pParti[ii]->m_Velocity.z;
	}
}

void Solver::Initialize(const int &numOfParti, std::vector<Particle*> pParti,std::vector<Force*> pForces, std::vector<Constraint*> pConstraints)
{
	
	this->_numOfParti= numOfParti;
	this->_pParti = pParti;
	this->_pForces = pForces;
	this->_pConstraints = pConstraints;
	
	
	this->_constSolver = new ConstraintSolver(this);
	
	this->DIM = _numOfParti*6;
}

void Solver::getDerivative(DATA* dst)
{

	for(int ii=0; ii<this->_pParti.size(); ii++)
	{
		_pParti[ii]->m_ForceAccumulator = make_vector(0.0f,0.0f,0.0f);
	}


	for(int ii=0; ii<this->_pForces.size();ii++)
	{
		_pForces[ii]->apply_force();
	}


	this->_constSolver->Solve();
	
	for(int ii=0; ii<this->_pParti.size();ii++)
	{
		int particleIndex = ii;
		//int phaseIndex = particleIndex*6;
		*(dst+6*ii)   = _pParti[particleIndex]->m_Velocity.x;
		*(dst+1+6*ii) = _pParti[particleIndex]->m_Velocity.y;
		*(dst+2+6*ii) = _pParti[particleIndex]->m_Velocity.z;

		Vector<DATA,3> acc = (_pParti[particleIndex]->m_ForceAccumulator/
							  _pParti[ii]->m_Mass);
		*(dst+3+6*ii) = acc.x;
		*(dst+4+6*ii) = acc.y;
		*(dst+5+6*ii) = acc.z;
	}
}


