
#include <Particle/tmp/Particle.hpp>
#include <Particle/tmp/Constrain.hpp>

class PointConstrain : public Constrain
{

public:
	PointConstrain(Particle *p1);
	virtual void draw();
	virtual void apply_Constrain();

private:
	Particle* _pConstrainedParticle; //particle1
	Vector3f _orginalPos;
};

