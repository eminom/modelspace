

#ifndef _PARTICLE_TEX_DEF__
#define _PARTICLE_TEX_DEF__

#include <list>
#include <glm/glm.hpp>
#include "utils/memo_pool.hpp"
#include "utils/gl_object_wrappers.hpp"
#include "utils/raw_object_wrappers.hpp"
#include "nodes/EsNode.h"

class ParticleTex:public EsNode
{
	// CPU representation of a particle
	struct Particle
	{
		glm::vec3 pos, speed;
		unsigned char r, g, b, a; // Color
		float size, angle, weight;
		float life; // Remaining life of the particle. if <0 : dead and unused.
		float cameradistance; // *Squared* distance to the camera. if dead : -1.0f
	};
private:
	ParticleTex();

public:
	static ParticleTex* create(int particle_count=1000);
	void setTexture(DD::Texture &t);

private:
	virtual void update(float dt)override;
	virtual void render(const glm::mat4 &transform)override;
	virtual bool init()override;

private:
	MemoPool<Particle> pool_;
	std::list<Particle*> particles_;
	DD::ArrayBuffer vbo_;
	DD::ArrayBuffer color_;
	DD::ArrayBuffer poss_;

	GLuint u_CAMERA_RIGHT;
	GLuint u_CAMERA_UP;
	GLuint u_VP;
	GLuint u_ID;

	DD::RawMemo<GLfloat> color_ptr_;
	DD::RawMemo<GLfloat> poss_ptr_;

	int max_particle_;
	DD::Texture tex_;
};


#endif