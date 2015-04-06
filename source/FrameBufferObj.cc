
#include "FrameBufferObj.h"
#include <GL/glew.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>

FrameBufferObj::operator bool()const
{
	return framebuffer_ && texture_ && renderbuffer_;
}

GLuint FrameBufferObj::texture()const{
	return texture_;
}

GLuint FrameBufferObj::framebuffer()const{
	return framebuffer_;
}

FrameBufferObj FrameBufferObj::create(int width, int height)
{
	FrameBufferObj rv;

	//
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	rv.framebuffer_.take(FramebufferName);

	// The texture we're going to render to
	GLuint renderedTexture = 0;
	glGenTextures(1, &renderedTexture);	
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	rv.texture_.take(renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLuint depthrenderbuffer = 0;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	rv.renderbuffer_.take(depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "Error creating frame-buffer, better clean it up before you regret\n");
	}

	//So the caller don't have to call this each time we create a framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);   //~Restore
	return rv;
}