

#include "exshader.h"
#include <cassert>
#include <common/shader.hpp>

ExShader::ExShader()
	:_theProgram(0)
{

}

ExShader::~ExShader()
{
	if(_theProgram)
	{
		glDeleteProgram(_theProgram);
	}
}

ExShader* ExShader::create(const char *vertexShader, const char *fragmentShader)
{
	auto rv = new ExShader;
	rv->init(vertexShader, fragmentShader);
	return rv;
}

void ExShader::init(const char *vertexShader, const char *fragmentShader)
{
	_theProgram = LoadShaders(vertexShader, fragmentShader);
	assert(_theProgram);
}

void ExShader::use()
{
	glUseProgram(_theProgram);
}

GLint ExShader::getLocation(const char *name)
{
	//if(!_theProgram){
	//	return -1;
	//}
	return glGetUniformLocation(_theProgram, name); 
}