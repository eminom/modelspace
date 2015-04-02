
#include "BlenderNode.h"
#include "common/shader.hpp"
#include "model/ObjectModel.h"

BlenderNode::BlenderNode()
{
	_faceDefaultColor = DDColor(1,0,0,0.5);
	_mvpSlot = -1;
	_indexVBO = 0;
}

void BlenderNode::setFaceDefaultColor(const DDColor &color)
{
	_faceDefaultColor = color;
}

void BlenderNode::setFaceDefaultColor(float r, float g, float b, float a){
	setFaceDefaultColor(DDColor(r,g,b,a));
}

BlenderNode::~BlenderNode()
{
	if(_indexVBO){
		glDeleteBuffers(1, &_indexVBO);
	}
}

BlenderNode* BlenderNode::create(const char *name, bool complex)
{
	auto rv = new BlenderNode;
	rv->init(name, complex);
	return rv;
}

bool BlenderNode::init(const char *name, bool complex)
{
	if(!complex){
		ObjectModel::loadFromJson(name, _vertex, _indice);
	} else {
		ObjectModel::loadFromJsonWithoutNormal(name, _vertex, _indice);
	}
	_program = ShaderCacheEx::instance()->shaderFor(ShaderType::Simple);
	_mvpSlot = _program->getLocation("u_mvp");

	glGenBuffers(1, &_vbo);
	assert(_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertex[0])*_vertex.size(), &_vertex[0], GL_STATIC_DRAW);

	glGenBuffers(1, &_indexVBO);
	assert(_indexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indice[0])*_indice.size(), &_indice[0], GL_STATIC_DRAW);
	return true;
}

void BlenderNode::draw(const glm::mat4 &matrix)
{
	glm::mat4 mvp = matrix * _model;
	_program->use();
	glUniformMatrix4fv(_mvpSlot, 1, GL_FALSE, &mvp[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glVertexAttrib4f(1, _faceDefaultColor.r, _faceDefaultColor.g, _faceDefaultColor.b, _faceDefaultColor.a);	//Red
	glDisableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexVBO);
	glDrawElements(GL_TRIANGLES, _indice.size(), GL_UNSIGNED_SHORT, 0);
}

