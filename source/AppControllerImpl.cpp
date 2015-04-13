

#include "AppController.h"


void AppController::test()
{
	// Not opened yet??
	GLint multisampling = 0;
	glGetIntegerv(GL_SAMPLE_BUFFERS , &multisampling);
	if (1==multisampling) {
		printf("Multi-sampling is enabled");
		glEnable(GL_MULTISAMPLE);

		GLint sampleCount = 0;
		glGetIntegerv(GL_SAMPLES, &sampleCount);
		for(int i=0;i<sampleCount;++i){
			GLfloat pos[4] = {0};   // a float pair indicating the offsets
			auto ptr = glGetMultisamplefv; // void(unsigned int, unsigned int, float *)
			if(ptr) {
				// Which is missing on window platform. (Mine)
				glGetMultisamplefv(GL_SAMPLE_POSITION, i, pos);
				printf("Sample[%d] offset is (%g, %g)\n", i, pos[0], pos[1]);
			}
		}
	}

	if( glIsEnabled(GL_SCISSOR_TEST) ){
		printf("Scissor test is enabled\n");
		GLint box[4];
		glGetIntegerv(GL_SCISSOR_BOX, box);
        
	} else {
		printf("Scissor test is disabled\n");
		//glEnable(GL_SCISSOR_TEST);
        //GLint box[4] = {0};
        //glGetIntegerv(GL_SCISSOR_BOX, box);
		//int h_margin = 0;
		//int v_margin = 0;
		//glScissor(h_margin, v_margin, screen_width_-h_margin * 2, screen_height_ - v_margin * 2);
        //memset(box, 0, sizeof(box));
        //glGetIntegerv(GL_SCISSOR_BOX, box);
        //printf("%f, %f,  %f, %f\n", box[0], box[1], box[2], box[3]);
	}

	/*  // Weird.
	if(glIsEnabled(GL_COLOR_LOGIC_OP)){
		glLogicOp(GL_OR);
	} else {
		printf("Logic op is disabled by default\n");
		glEnable(GL_COLOR_LOGIC_OP);
		assert(glIsEnabled(GL_COLOR_LOGIC_OP));
		glLogicOp(GL_COPY);
	}
	*/
}


bool AppController::prepareFramebuffer()
{
	framebuffer_ = FrameBufferObj::create(screen_width_, screen_height_);
	if(!framebuffer_){
		fprintf(stderr, "Framebuffer failed to create\n");
		return false;
	}
	CheckGL()
	GLfloat vertice[][2]={
		{-1, -1},
		{1,  -1},
		{-1, 1},
		{-1, 1},
		{1, -1},
		{1, 1}
	};
	vertex_.make_and_bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertice), vertice, GL_STATIC_DRAW);
	return true;
}


void AppController::startFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_.framebuffer());
	//glViewport(0,0, screen_width_, screen_height_);
	// And drawing to my place
}

void AppController::finalizeFramebuffer()
{
	ExShader *shader = ShaderCacheEx::instance()->shaderFor(ShaderType::Wobbler);
	GLuint time = shader->getLocation("time");
	GLuint tex = shader->getLocation("tex");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glViewport(0,0, screen_width_, screen_height_);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, framebuffer_.texture());

	shader->use();
	glUniform1f(time, glfwGetTime()*10.0f);
	glUniform1i(tex, 0);	//~ texture unit0

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
}