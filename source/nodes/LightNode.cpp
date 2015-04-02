

#include "LightNode.h"
#include "model/ObjectModel.h"
#include "common/shader.hpp"
#include <cassert>

const float pi = acosf(0) * 2;

LightNode::LightNode()
{
	_timeScale = 1;
	_mode = nullptr;
}

bool LightNode::init(const DDPoint &pos)
{
	// Pre-init by blender-normal
	_position = pos;
	setTransform(_position.x, _position.y, _position.z);

	_radius = glm::length(_position);
	_hAngle = atanf( _position.x / _position.z);
	_vAngle = acosf( (_position.x * _position.x + _position.z * _position.z) / (_position.x * _position.x + _position.y * _position.y 
		+ _position.z * _position.z ));

	_mode = BlenderNormal::create("objects/uvsphere8.json", false);
	_mode->setFaceDefaultColor(DDColor_White);
	return true;
}

LightNode* LightNode::create(float x, float y, float z)
{
	//auto rv = BlenderNormal::create<LightNode>("objects/uvsphere8.json", false);
	//rv->init(DDPoint(x,y,z));
	//rv->setFaceDefaultColor(DDColor(1,1,1,1));
	//return rv;

	auto rv = new LightNode;
	rv->init(DDPoint(x,y,z));
	return rv;
}
//
//void LightNode::draw(const glm::mat4 &mat)
//{
//	glm::mat4 mvp = mat * _model;
//	glUseProgram(_program);
//	glUniformMatrix4fv(_mvpSlot, 1, GL_FALSE, &mvp[0][0]);
//	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
//
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GridPoint), (void*)offsetof(GridPoint, position));
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GridPoint), (void*)offsetof(GridPoint, color));
//
//	glDrawArrays(GL_TRIANGLES, 0, _vertex.size());
//}

void LightNode::draw(const glm::mat4 &vp)
{
	(static_cast<EsNode*>(_mode))->draw(vp * _model);
}

DDPoint LightNode::position()
{
	return _position;
}

void LightNode::update(float dt)
{
	dt *= _timeScale;
	_hAngle = fmodf(_hAngle + dt * 1.50f,2 * pi);
	_vAngle = fmodf(_vAngle + dt * 1.201, 2 * pi);
	_position = DDPoint(
		_radius * cosf(_vAngle) * sinf(_hAngle),
		_radius * sinf(_vAngle),
		_radius * cos(_vAngle) * cos(_hAngle)
	);
	setTransform(_position.x, _position.y, _position.z);
}

void LightNode::setTimeScale(float scale)
{
	_timeScale = scale;
}