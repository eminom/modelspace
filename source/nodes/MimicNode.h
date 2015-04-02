

#ifndef _MIMIC_NODE_DEF__
#define _MIMIC_NODE_DEF__

#include "EsNode.h"

class MimicNode:public EsNode
{
protected:
	MimicNode();
	~MimicNode();

public:
	static MimicNode* create();	// Sample.

private:
	bool init()override;
	void draw(const glm::mat4 &mat)override;

private:
	GLuint _indexVBO;
	GLuint _colorVBO;
	GLuint _fixVBO;

	std::vector<glm::vec3> _vertex;
	std::vector<GLushort> _indice;
	
	GLint _vpSlot;
	GLint _tSlot;
	GLint _sSlot;
};



#endif