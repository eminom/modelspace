


#ifndef _DIRECTOR_DEF__
#define _DIRECTOR_DEF__

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "nodes/EsNode.h"
#include "nodes/graphics/DDGraphics.h"
#include "LightProtocol.h"
#include "utils/gl_object_wrappers.hpp"
#include <vector>

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
	int lightCount()const;
	DDPoint getLightPosition(int index);
	void setLight(LightProtocol *light);

public:
	static EsDirector* instance();
	static void purge();

private:
	EsNode *_root;
	DD::VertexArrayObject _vao;
	std::vector<LightProtocol*> _light;
};

#endif