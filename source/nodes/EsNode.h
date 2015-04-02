


#ifndef _ES_NODE__DEF__
#define _ES_NODE__DEF__

#include <vector>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "shaderman/shadercache.h"

class EsNode
{
protected:
	EsNode();
	virtual ~EsNode();

public:
	virtual void render(const glm::mat4 &transform);
	virtual void addSub(EsNode *sub);
	virtual void setColor(float r, float g, float b, float a);

	virtual bool init();
	void destroy();
	void setTransform(const glm::mat4 &trans);
	void setScale(float x);
	void setScale(float xs, float ys, float zs);
	void setTransform(float xf, float yf, float zf);
	void setRotate(float radians, float x, float y, float z);
	void setVisible(bool visible);

	void synthesizeMatrix();
	void doUpdate(float dt);

protected:
	virtual void update(float dt);

public:
	virtual void draw(const glm::mat4 &transform);

protected:
	std::vector<EsNode*> _subs;

	glm::mat4 _model;
	glm::mat4 _rotate;
	glm::mat4 _scale;
	glm::mat4 _translate;

	ExShader *_program;
	GLuint _vbo;
	bool _visible;
};

#endif