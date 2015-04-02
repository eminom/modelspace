

#ifndef _BOX_NODE_DEF__
#define _BOX_NODE_DEF__

#include "Common.h"
#include "GridNode.h"

class BoxNode:public GridNode
{
private:
	BoxNode();

public:

	//vheight: along the y axis
	//width:   along the x axis
	//height:  along the z axis
	static BoxNode* create(float x, float y, float width = 1, float height = 1, float vheight = 1);
	void shiftXZ();

protected:
	bool init(float x, float y, float width, float height, float vheight);
	void draw(const glm::mat4 &matrix)override;

private:
	float _width, _height, _vheight;
	GLuint _mvpSlot;
	int _vertexCount;
};


#endif