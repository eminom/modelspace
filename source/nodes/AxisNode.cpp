

#include "AxisNode.h"
#include <common/shader.hpp>

AxisNode::AxisNode()
{
	_mvpSlot = -1;

}

AxisNode* AxisNode::create()
{
	auto rv = new AxisNode;
	rv->init();
	return rv;
}

bool AxisNode::init()
{
	_program = ShaderCacheEx::instance()->shaderFor(ShaderType::Simple);
	_mvpSlot = _program->getLocation("u_mvp");

	VGridPointEx vp;
	vp.add(-1,0,0,DDColor(1,0,0,1));
	vp.add(1,0,0,DDColor(1,0,0,1));
	vp.add(0,-1,0,DDColor(0,0,1,1));
	vp.add(0,1, 0 , DDColor(0,0,1,1));
	vp.add(0,0, -1, DDColor(0,1,0,1));
	vp.add(0,0, 1, DDColor(0,1,0,1));
	setScale(10);
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vp.size() * sizeof(vp[0]), &vp[0], GL_STATIC_DRAW);
	return true;
}

void AxisNode::draw(const glm::mat4 &mat)
{
	glm::mat4 fin = mat * _model;
	_program->use();
	glUniformMatrix4fv(_mvpSlot, 1, GL_FALSE, &fin[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GridPoint), (void*)offsetof(GridPoint, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GridPoint), (void*)offsetof(GridPoint, color));
	glDrawArrays(GL_LINES, 0, 6);
}