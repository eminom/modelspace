

#include "BlenderNormal.h"
#include "common/shader.hpp"
#include "model/ObjectModel.h"
#include "Director.h"



BlenderNormal::BlenderNormal()
{
	for(int i=0;i<sizeof(_lightPosSlot)/sizeof(*_lightPosSlot);++i){
		_lightPosSlot[i] = -1;
	}
	_modelSlot = -1;
	_normalMatrixSlot = -1;
	_mvpSlot = -1;

	_type = BlenderNormalType::Error;
}

BlenderNormal* BlenderNormal::create(const char *path, bool complex)
{
	auto rv= new BlenderNormal;
	rv->init(path, complex);
	return rv;
}

bool BlenderNormal::init(const char *path, bool complex){
	if(complex){
		_type =  BlenderNormalType::Complex;
		return initComplex(path);
	} else {
		_type = BlenderNormalType::Simple;
		return initSimple(path);
	}
}

void BlenderNormal::loadLightUniforms(){
	for(int i=0;i<sizeof(_lightPosSlot)/sizeof(*_lightPosSlot);++i){
		char str[BUFSIZ];
		sprintf(str, "u_LightPosition%d", i);
		_lightPosSlot[i] = _program->getLocation(str);
		//assert(_lightPosSlot[i]>=0);
	}
}

void BlenderNormal::setLightUniforms(){
	int lsize = EsDirector::instance()->lightCount();
	for(int i=0;i<lsize;++i){
		if(_lightPosSlot[i]>=0){
			DDPoint lightPos = EsDirector::instance()->getLightPosition(i);
			glUniform3f(_lightPosSlot[i], lightPos.x, lightPos.y, lightPos.z);
		}
	}
}

bool BlenderNormal::initComplex(const char *path){
	_program = ShaderCacheEx::instance()->shaderFor(ShaderType::Diffuse);
	loadLightUniforms();
	_modelSlot = _program->getLocation("u_ModelMatrix");
	_normalMatrixSlot = _program->getLocation("u_NormalMatrix");
	_mvpSlot = _program->getLocation("u_MVP");
	ObjectModel::loadFromJsonWithNormal(path, _vertex, _normal, _colors);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertex[0])*_vertex.size(), &_vertex[0], GL_STATIC_DRAW);
	
	GLuint normal = 0;
	glGenBuffers(1, &normal);
	glBindBuffer(GL_ARRAY_BUFFER, normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_normal[0])*_normal.size(), &_normal[0], GL_STATIC_DRAW);
	_normalVBO.take(normal);

	GLuint color = 0;
	glGenBuffers(1, &color);
	glBindBuffer(GL_ARRAY_BUFFER, color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_colors[0]) * _colors.size(), &_colors[0], GL_STATIC_DRAW);
	_colorVBO.take(color);
	return true;
}

bool BlenderNormal::initSimple(const char *path)
{
	return BlenderNode::init(path, true);
}

void BlenderNormal::setColor(const DDColor&color)
{
	for(auto &one:_colors){
		one = color;
	}
	glBindBuffer(GL_ARRAY_BUFFER, _colorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_colors[0])*_colors.size(), &_colors[0], GL_STATIC_DRAW);//update
}

void BlenderNormal::draw(const glm::mat4 &mat){
	switch(_type){
	case BlenderNormalType::Simple:
		drawSimple(mat);
		break;
	case BlenderNormalType::Complex:
		drawComplex(mat);
		break;
	}
}

void BlenderNormal::setColor(float r, float g, float b, float a){
	setColor(DDColor(r,g,b,a));
}

void BlenderNormal::drawSimple(const glm::mat4 &mat)
{
	BlenderNode::draw(mat);
}

void BlenderNormal::drawComplex(const glm::mat4 &mat) {
	glm::mat4 mvp = mat * _model;
	_program->use();

	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(_model)));

	//glUniform4f(_lightPosSlot, 0.5, 1.25, 0.3, 1);
	setLightUniforms();

	glUniformMatrix4fv(_modelSlot, 1, GL_FALSE, &_model[0][0]);
	glUniformMatrix4fv(_mvpSlot, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix3fv(_normalMatrixSlot, 1, GL_FALSE, &normalMatrix[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//glVertexAttrib4f(1, 0.3, 0.5, 0.5, 1);
	//glDisableVertexAttribArray(1);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _normalVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, _colorVBO);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//
	glDrawArrays(GL_TRIANGLES, 0, _vertex.size());
}

