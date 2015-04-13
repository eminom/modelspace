

#ifndef _EX_SHADER_DEF__
#define _EX_SHADER_DEF__

#include <GL/glew.h>

class ExShader
{
protected:
	ExShader();
	~ExShader();

public:
	static ExShader *create(const char *vertexShader, const char *fragmentShader);
	void use();
	GLint getLocation(const char *uniformName);
	GLuint program()const;

private:
	void init(const char *vertexShader, const char *fragmentShader);

private:
	GLuint _theProgram;
	friend class ShaderCacheEx;
};

#endif