

#ifndef _AXISNODE_DEF__
#define _AXISNODE_DEF__

#include "EsNode.h"
#include "graphics/DDGraphics.h"

class AxisNode:public EsNode
{
private:
	AxisNode();

public:
	static AxisNode* create();

private:
	bool init()override;
	void draw(const glm::mat4 &)override;

private:
	GLint _mvpSlot;
};


#endif