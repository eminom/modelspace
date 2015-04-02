


#ifndef _DIRECTOR_DEF__
#define _DIRECTOR_DEF__

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "nodes/EsNode.h"
#include "nodes/graphics/DDGraphics.h"
#include "LightProtocol.h"

class EsDirector
{
protected:
	EsDirector();
	virtual ~EsDirector();
	void init();
	void deinit();

public:
	void setRoot(EsNode *node);
	EsNode* root()const;
	void render(const glm::mat4& MVP);
	void updateNode(float dt);
	DDPoint getLightPosition();
	void setLight(LightProtocol *light);

public:
	static EsDirector* instance();
	static void purge();

private:
	EsNode *_root;
	GLuint _vao;
	LightProtocol *_light;
};

#endif