

#ifndef _FIELDNODE__DEF__
#define _FIELDNODE__DEF__

#include "graphics/DDGraphics.h"
#include "Common.h"
#include "PrimitiveNode.h"
#include "GridNode.h"
#include <vector>

class FieldNode:public GridNode
{
private:
	FieldNode();

public:
	static FieldNode* create(int ac);

protected:
	virtual void draw(const glm::mat4 &transform)override;
	bool init(int ac);

private:
	GLint _mvpSlot;
};

#endif