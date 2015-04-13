




#ifndef _FLAG_SPRITE_DEF__
#define _FLAG_SPRITE_DEF__

#include "nodes/EsNode.h"
#include "utils/gl_object_wrappers.hpp"

class FlagSprite:public EsNode
{
private:
	FlagSprite();

public:
	static FlagSprite* create(const char *path);

private:
	virtual void update(float dt)override;
	virtual bool init()override;
	virtual void draw(const glm::mat4 &VP)override;

private:
	void genMesh();
	void genMeshSub(glm::vec2 p0, glm::vec2 p1, int dep);

private:
	std::vector<glm::vec2> _mesh;
	int maxDepth_;

	GLuint u_Center;
	GLuint u_CameraRight;
	GLuint u_CameraUp;
	GLuint u_VP;
	GLuint u_ID;

	DD::Texture tex_;
};

#endif