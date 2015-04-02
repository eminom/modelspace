

#ifndef _BLENDER_NODE_DEF__
#define _BLENDER_NODE_DEF__

#include "EsNode.h"
#include "graphics/DDGraphics.h"

class BlenderNode:public EsNode
{
protected:
	BlenderNode();
	~BlenderNode();

public:
	static BlenderNode* create(const char *name, bool complex = false);
	void setFaceDefaultColor(const DDColor&color);
	void setFaceDefaultColor(float r, float g, float b, float a);

protected:
	bool init(const char *name, bool complex);
	void draw(const glm::mat4 &matrix)override;

protected:
	VDDPointEx _vertex;
	int _mvpSlot;

private:
	std::vector<GLushort> _indice;
	GLuint _indexVBO;
	DDColor _faceDefaultColor;
};


#endif