

#include "GridNode.h"

GridNode::GridNode()
{
	_vertexCount = 0;
}

void GridNode::setColor(const DDColor &color)
{
	for(auto it=_grids.begin();it!=_grids.end();++it) {
		GridPoint &gp = *it;
		gp.color = color;
	}

	assert( _vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_grids[0]) * _grids.size(), &_grids[0], GL_STATIC_DRAW);
	_vertexCount = _grids.size();
}

void GridNode::setColor(float r,float g, float b, float a){
	setColor(DDColor(r,g,b,a));
}