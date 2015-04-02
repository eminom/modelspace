


#include "FieldNode.h"
#include <common/shader.hpp>
#include "model/ObjectModel.h"

FieldNode::FieldNode()
{
	_vertexCount = 0;
	_mvpSlot = -1;
}

FieldNode* FieldNode::create(int ac)
{
	FieldNode *rv = new FieldNode;
	rv->init(ac);
	return rv;
}

void FieldNode::draw(const glm::mat4 &matrix)
{
	glm::mat4 mat = matrix * _model;
	_program->use();
	glUniformMatrix4fv(_mvpSlot, 1, GL_FALSE, &mat[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	//positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GridPoint), (void*)offsetof(GridPoint, position));

	//colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GridPoint), (void*)offsetof(GridPoint, color));

	glDrawArrays(GL_LINES, 0, _vertexCount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

bool FieldNode::init(int ac)
{
	_program = ShaderCacheEx::instance()->shaderFor(ShaderType::Simple);
	_mvpSlot = _program->getLocation("u_mvp");
	assert( _mvpSlot >= 0);

	ObjectModel::genGrids_third(_grids, -1, 1, ac);

	assert( !_vbo);
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_grids[0])*_grids.size(), &_grids[0], GL_DYNAMIC_DRAW);
	_vertexCount = _grids.size();
	synthesizeMatrix();
	return true;	
}
