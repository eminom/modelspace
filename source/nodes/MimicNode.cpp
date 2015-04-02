

#include "MimicNode.h"
#include "model/ObjectModel.h"
#include "common/shader.hpp"

MimicNode::MimicNode()
{
	_vpSlot = -1;
	_tSlot = -1;
	_sSlot = -1;
	_indexVBO = 0;
	_colorVBO = 0;
	_fixVBO = 0;
}

MimicNode::~MimicNode()
{
	if(_indexVBO){
		glDeleteBuffers(1, &_indexVBO);
	}
	if(_vbo){
		glDeleteBuffers(1, &_colorVBO);
	}
	if(_fixVBO){
		glDeleteBuffers(1, &_fixVBO);
	}
}

MimicNode* MimicNode::create()
{
	auto rv = new MimicNode;
	rv->init();
	return rv;
}

bool MimicNode::init()
{
	_program = ShaderCacheEx::instance()->shaderFor(ShaderType::Mimic);
	_vpSlot = _program->getLocation("u_vp");
	_tSlot  = _program->getLocation("u_m");
	_sSlot  = _program->getLocation("u_s");
	ObjectModel::loadFromJson("objects/cube.json", _vertex, _indice);

	////
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertex[0]) * _vertex.size(), &_vertex[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &_indexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indice[0]) * _indice.size(), &_indice[0], GL_STATIC_DRAW);

	// Just 2
	VDDColorEx colors;
	colors.add(1,0,0,0.5);
	colors.add(0,1,0,0.5);
	colors.add(0,0,1,0.5);
	colors.add(1,0,1,0.5);
	colors.add(1,1,0, 0.5);

	VDDPointEx fix;
	fix.add(0,0,0);
	fix.add(0.5, 0, 0);
	fix.add(-0.5,0,0);
	fix.add(0, 0, 0.5);
	fix.add(0,0,-0.5);

	glGenBuffers(1, &_colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _colorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors[0])*colors.size(), &colors[0], GL_STREAM_DRAW);

	glGenBuffers(1, &_fixVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _fixVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fix[0])*fix.size(), &fix[0], GL_STREAM_DRAW);

	return true;
}

void MimicNode::draw(const glm::mat4 &mat)
{
	_program->use();
	glUniformMatrix4fv(_vpSlot, 1, GL_FALSE, &mat[0][0]);
	glUniformMatrix4fv(_tSlot,  1, GL_FALSE, &_translate[0][0]);
	glUniformMatrix4fv(_sSlot,  1, GL_FALSE, &_scale[0][0]);

	glVertexAttribDivisor(0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glVertexAttribDivisor(1, 1);
	glBindBuffer(GL_ARRAY_BUFFER, _colorVBO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	
	glVertexAttribDivisor(2, 1);
	glBindBuffer(GL_ARRAY_BUFFER, _fixVBO);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexVBO);
	glDrawElementsInstanced(GL_TRIANGLES, _indice.size(), GL_UNSIGNED_SHORT, 0, 5);

	//GLenum errCode = glGetError();
	//int x = errCode;

	glVertexAttribDivisor(0, 0);	//~ It is not necessary to restore so.
	glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 0);
}



