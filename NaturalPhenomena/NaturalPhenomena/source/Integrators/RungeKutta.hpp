#ifndef _RUNGEKUTTA_HPP_
#define _RUNGEKUTTA_HPP_

#include <Integrators\Integrator.hpp>
#include <Integrators\Solver.hpp>
class RungeKutta:public Integrator
{

public:

	RungeKutta(Solver* solver);
	virtual ~RungeKutta();
	//virtual void Integrate(Vector3f acc, Particle* pParticle, float elaspedTime);
	virtual void Integrate(DATA elaspedTime);
	virtual void Initialize();
private:
	DATA* _k1;
	DATA* _k2;
	DATA* _k3;
	DATA* _k4;

	DATA* _midPhaseSpace1;
	DATA* _midPhaseSpaceDot1;
	DATA* _midPhaseSpace2;
	DATA* _midPhaseSpaceDot2;
	DATA* _midPhaseSpace3;
	DATA* _midPhaseSpaceDot3;

};
#endif
