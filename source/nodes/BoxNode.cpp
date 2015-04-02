

#include "graphics/DDGraphics.h"

#include "BoxNode.h"
#include "glm/gtx/transform.hpp"

#include "model/ObjectModel.h"
#include <vector>

#include "common/shader.hpp"

BoxNode::BoxNode()
{
	_mvpSlot = -1;
}

BoxNode* BoxNode::create(float x, float y, float width, float height, float vheight)
{
	BoxNode *rv = new BoxNode;
	rv->init(x, y, width, height, vheight);
	return rv;
}

void BoxNode::shiftXZ()
{
	// the scale y
	float yShift = _vheight / 2 * _scale[1][1];
	_translate = glm::translate(_translate, glm::vec3(0, yShift, 0));
	synthesizeMatrix();
}

bool BoxNode::init(float x, float y, float width, float height, float vheight)
{
	_program = ShaderCacheEx::instance()->shaderFor(ShaderType::Simple);
	_mvpSlot = _program->getLocation("u_mvp");

	_width = width;
	_height= height;
	_vheight=vheight;

	assert( _mvpSlot >= 0 );
	_translate = glm::translate(glm::vec3(x, 0, -y));

	ObjectModel::genBox(_grids, width, height, vheight);

	assert (0==_vbo);
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_grids[0]) * _grids.size(), &_grids[0], GL_DYNAMIC_DRAW);

	_vertexCount = _grids.size();
	synthesizeMatrix();
	return true;
}

void BoxNode::draw(const glm::mat4 &matrix)
{
	glm::mat4 mat = matrix * _model;
	_program->use();
	glUniformMatrix4fv(_mvpSlot, 1, GL_FALSE, &mat[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GridPoint), (void*)offsetof(GridPoint, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GridPoint), (void*)offsetof(GridPoint, color));
	glDrawArrays(GL_LINES, 0, _vertexCount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
