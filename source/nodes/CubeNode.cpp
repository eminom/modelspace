

#include "graphics/DDGraphics.h"

#include "CubeNode.h"
#include "glm/gtx/transform.hpp"

#include "model/ObjectModel.h"
#include <vector>

#include "common/shader.hpp"
#include "interop/access/lua_access.h"

extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "LuaScriptor.h"

CubeNode::CubeNode()
	:_simpleNode(nullptr)
{
	_mvpSlot = -1;
}

CubeNode::~CubeNode()
{
	delete _simpleNode;
}

CubeNode* CubeNode::create(float x, float y, float width, float height, float vheight)
{
	CubeNode *rv = new CubeNode;
	rv->init(x, y, width, height, vheight);
	return rv;
}

void CubeNode::shiftXZ()
{
	// the scale y
	float yShift = _vheight / 2 * _scale[1][1];
	_translate = glm::translate(_translate, glm::vec3(0, yShift, 0));
	synthesizeMatrix();
}

bool CubeNode::init(float x, float y, float width, float height, float vheight)
{
	_program = ShaderCacheEx::instance()->shaderFor(ShaderType::Simple);
	_mvpSlot = _program->getLocation("u_mvp");

	_width = width;
	_height= height;
	_vheight=vheight;

	assert( _mvpSlot >= 0 );
	_translate = glm::translate(glm::vec3(x, 0, -y));

	ObjectModel::genSolidBox(_grids, width, height, vheight);

	assert (0==_vbo);
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_grids[0]) * _grids.size(), &_grids[0], GL_DYNAMIC_DRAW);

	_vertexCount = _grids.size();
	synthesizeMatrix();

	assert(!_simpleNode);
	_simpleNode = new RefObject(LuaScriptor::sharedInstance()->getLuaState());
	_simpleNode->requireFuncDo("exec/SimpleNode.lua");
	_simpleNode->execVoid("setHost","v", this);
	_simpleNode->execVoid("setPos", "fff", x,0.0f,-y);
	_simpleNode->execVoid("update", "f", 0.0f);
	return true;
}

void CubeNode::draw(const glm::mat4 &matrix)
{
	glm::mat4 mat = matrix * _model;
	_program->use();
	glUniformMatrix4fv(_mvpSlot, 1, GL_FALSE, &mat[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GridPoint), (void*)offsetof(GridPoint, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GridPoint), (void*)offsetof(GridPoint, color));
	glDrawArrays(GL_TRIANGLES, 0, _vertexCount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}


void CubeNode::update(float dt)
{
	_simpleNode->execVoid("update", "f", dt);
}