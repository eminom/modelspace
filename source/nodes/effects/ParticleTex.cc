



//The contents origninates from OpenGL-tutorial Chapter 18: Particles(by instancing)

#include "ParticleTex.h"
#include "shaderman/shadercache.h"
#include "AppController.h"
#include "utils/state_saver.h"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

ParticleTex::ParticleTex()
{
	u_CAMERA_RIGHT = -1;
	u_CAMERA_UP = -1;
	u_VP = -1;
	u_ID = -1;

	max_particle_ = 0;
}

ParticleTex* ParticleTex::create(int pc)
{
	ParticleTex* rv = new ParticleTex;
	rv->max_particle_ = pc;
	rv->init();
	return rv;
}

bool ParticleTex::init()
{
	EsNode::init();
	_program = ShaderCacheEx::instance()->shaderFor(ShaderType::ParticleTex);
	u_CAMERA_RIGHT = _program->getLocation("CameraRight_worldspace");
	u_CAMERA_UP    = _program->getLocation("CameraUp_worldspace");
	u_ID = _program->getLocation("id");
	u_VP = _program->getLocation("VP");

	const GLfloat vertice[][2]={
		{-0.5, -0.5},
		{0.5, -0.5},
		{-0.5, 0.5},
		{0.5, 0.5}
	};
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertice), vertice, GL_STATIC_DRAW);
	vbo_.take_and_own(vbo);

	GLuint color = 0;
	glGenBuffers(1, &color);
	glBindBuffer(GL_ARRAY_BUFFER, color);
	glBufferData(GL_ARRAY_BUFFER, max_particle_ * sizeof(GLfloat) * 4, nullptr, GL_DYNAMIC_DRAW);
	color_.take_and_own(color);

	GLuint poss = 0;
	glGenBuffers(1, &poss);
	glBindBuffer(GL_ARRAY_BUFFER, poss);
	glBufferData(GL_ARRAY_BUFFER, max_particle_ * sizeof(GLfloat) * 4, nullptr, GL_DYNAMIC_DRAW);
	poss_.take_and_own(poss);

	color_ptr_.alloc(max_particle_*4);
	poss_ptr_.alloc(max_particle_*4);

	return true;
}

void ParticleTex::update(float dt)
{
	//dt *= 0.1;
	int newparticles = (int)(dt*10000.0);
	if (newparticles > max_particle_ - particles_.size()){
		newparticles = max_particle_ - particles_.size();
	}

	// Making new ones
	for(int i=0; i<newparticles; i++){
		Particle *one = pool_.alloc();
		particles_.push_back(one);

		one->life = rand() % 15 + 0.5f; // This particle will live 5 seconds.
		//one->pos = glm::vec3(0,0,-20.0f);
		one->pos = glm::vec3(0,0,0);

		float spread = 0.15f;
		glm::vec3 maindir = glm::vec3(0.0f, 1.0f, 0.0f);
		// Very bad way to generate a random direction; 
		// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
		// combined with some user-controlled parameters (main direction, spread, etc)
		glm::vec3 randomdir = glm::vec3(
			(rand()%2000 - 1000.0f)/1000.0f,
			(rand()%2000 - 1000.0f)/1000.0f,
			(rand()%2000 - 1000.0f)/1000.0f
			);

		one->speed = maindir + randomdir*spread;
		// Very bad way to generate a random color
		one->r = rand() % 256;
		one->g = rand() % 256;
		one->b = rand() % 256;
		//one->a = (rand() % 256) / 3;
		one->a = 255;
		one->size = (rand()%1000)/2000.0f * 0.3f + 0.3f;
	}

	int pcIndex = 0;
	// Simulate all particles
	for(auto it:particles_){
		Particle& p = *it; // shortcut
		p.life -= dt;
		p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)dt * 0.1f;
		p.pos += p.speed * (float)dt;
		//printf("position up to %f, %f, %f\n", p.pos.x, p.pos.y, p.pos.z);
		//p.cameradistance = glm::length2( p.pos - CameraPosition );
		// Fill the GPU buffer.
		poss_ptr_.take(4 * pcIndex, p.pos.x, p.pos.y, p.pos.z, p.size);
		color_ptr_.take(4 * pcIndex, p.r / 255.0f, p.g / 255.0f, p.b / 255.0f, p.a / 255.0f);
		pcIndex++;
	}

	//Free old ones:
	for(auto it=particles_.begin();it!=particles_.end();){
		Particle* one = *it;
		if(one->life <= 0){
			particles_.erase(it++);
			pool_.free(one);
		} else {
			++it;
		}
	}
	//printf("%d\n", pcIndex);

	// Update the buffers that OpenGL uses for rendering.
	// There are much more sophisticated means to stream data from the CPU to the GPU, 
	// but this is outside the scope of this tutorial.
	// http://www.opengl.org/wiki/Buffer_Object_Streaming

	glBindBuffer(GL_ARRAY_BUFFER, poss_);
	//glBufferData(GL_ARRAY_BUFFER, max_particle_ * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, max_particle_ * 4 * sizeof(GLfloat), poss_ptr_.ptr());

	glBindBuffer(GL_ARRAY_BUFFER, color_);
	//glBufferData(GL_ARRAY_BUFFER, max_particle_ * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, max_particle_ * 4 * sizeof(GLfloat), color_ptr_.ptr());
}

void ParticleTex::setTexture(DD::Texture &t)
{
	tex_ = t;
}

void ParticleTex::render(const glm::mat4 &transform)
{
	DD::StateSaver ss;
	glm::mat4 view = AppController::sharedInstance().getView();
	glm::mat4 projection = AppController::sharedInstance().getProjection();
	//glm::mat4 view = ViewMatrix;
	//glm::mat4 projection = ProjectionMatrix;
	glm::mat4 vp = projection * view;
	_program->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_);

	glUniform3f(u_CAMERA_RIGHT, view[0][0], view[1][0], view[2][0]);
	glUniform3f(u_CAMERA_UP,    view[0][1], view[1][1], view[2][1]);
	glUniform1i(u_ID, 0);	//texture unit 0 for 2D image. 
	glUniformMatrix4fv(u_VP, 1, GL_FALSE, &vp[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, poss_);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, color_);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particles_.size());
	glVertexAttribDivisor(0,0);
	glVertexAttribDivisor(1,0);
	glVertexAttribDivisor(2,0);
}


