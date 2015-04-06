
#ifndef _BLENDER_PARTICLE_DEF__
#define _BLENDER_PARTICLE_DEF__

#include "nodes/BlenderNormal.h"
#include "utils/memo_pool.hpp"
#include "utils/gl_object_wrappers.hpp"
#include <list>

class BlenderParticle:public BlenderNormal
{
	friend class BlenderNormal;
	struct BNode
	{
		glm::vec4 posfix;
		glm::vec4 speed;
		glm::vec4 color;
		float life;
	};

private:
	~BlenderParticle();
	BlenderParticle(int max, float fScale);

public:
	static BlenderParticle* create(const char *path, int max, float scale, float x, float y, float z);

private:
	virtual bool initComplex(const char *path)override;
	virtual void drawComplex(const glm::mat4 &mat)override;
	virtual void update(float dt)override;

private:
	MemoPool<BNode> pool_;
	std::list<BNode*> nodes_;
	int particleMax_;

	GLfloat *positionInfo_;
	GLfloat *colorInfo_;
	DD::ArrayBuffer posfix_;
	DDPoint bornPos_;
	float fScale_;
};


#endif