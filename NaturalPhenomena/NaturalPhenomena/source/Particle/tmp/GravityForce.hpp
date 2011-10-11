
#include <Particle/tmp/Particle.hpp>
#include <Particle/tmp//Force.hpp>
//#include <Cloth/Cloth.hpp>
class GravityForce : public Force
{

public:
	GravityForce(Particle *p1);

	virtual void draw();
	virtual void apply_force();

private:
	Particle *const m_p1; //particle1
};

