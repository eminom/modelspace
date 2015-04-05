

#ifndef _PARTICLE_EFFECTS_DEF__
#define _PARTICLE_EFFECTS_DEF__

#include <list>
#include <glm/glm.hpp>
#include "utils/memo_pool.hpp"
#include "utils/gl_object_wrappers.hpp"
#include "nodes/EsNode.h"

// CPU representation of a particle
struct Particle
{
	glm::vec3 pos, speed;
	unsigned char r, g, b, a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f
};

class ParticleNode:public EsNode
{
private:
	static const int MaxParticles = 1000;
	ParticleNode();
	~ParticleNode();

public:
	static ParticleNode* create();

private:
	virtual void update(float dt)override;
	virtual void render(const glm::mat4 &transform)override;
	virtual bool init()override;

private:
	MemoPool<Particle> pool;
	std::list<Particle*> particles_;
	DD::ArrayBuffer vbo_;
	DD::ArrayBuffer color_;
	DD::ArrayBuffer poss_;

	GLuint u_cameraRight_;
	GLuint u_cameraUp_;
	GLuint u_vp_;

	GLfloat *color_ptr_;
	GLfloat *poss_ptr_;
};


#endif