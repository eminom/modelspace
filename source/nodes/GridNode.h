

#ifndef _SHAPE_NODE_DEF__
#define _SHAPE_NODE_DEF__

#include "graphics/DDGraphics.h"
#include "EsNode.h"
#include "glm/glm.hpp"

class GridNode:public EsNode
{
protected:
	GridNode();

public:
	void setColor(const DDColor &color);

protected:
	void setColor(float r,float g, float b, float a)override;

protected:
	std::vector<GridPoint> _grids;
	int _vertexCount;
};

#endif