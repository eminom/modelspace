

#ifndef _CREATE_FRAMEBUFFER_DEF__
#define _CREATE_FRAMEBUFFER_DEF__

#include <GL/glew.h>
#include "utils/gl_object_wrappers.hpp"

class FrameBufferObj
{
public:
	operator bool()const;

	GLuint texture()const;
	GLuint framebuffer()const;

private:
	DD::Framebuffer framebuffer_;
	DD::Texture texture_;
	DD::Renderbuffer renderbuffer_;

public:
	static FrameBufferObj create(int width, int height);
};

#endif