

#include "FlagSprite.h"
#include "utils/state_saver.h"
#include "AppController.h"
#include "texture/TextureData.h"

FlagSprite::FlagSprite()
	:maxDepth_(5)
{
	u_Center = -1;
	u_CameraRight = -1;
	u_CameraUp = -1;
	u_VP = -1;
	u_ID = -1;
	u_StartAngle = -1;

	uStartAngle_ = 0.0f;
}

void FlagSprite::update(float dt)
{
	uStartAngle_ += dt;
}

FlagSprite* FlagSprite::create(const char *path)
{
	auto rv = new FlagSprite;
	rv->init();

	PngImageData pid;
	if( pid.load(path) ){
		TextureData td;
		if(!td.initWithPngData(pid)){
			fprintf(stderr,"Error loading png image into GL texture\n");
		}
		rv->tex_ = td.texture();	//~ Share one copy
	} else {
		fprintf(stderr, "Error loading image file %s\n", path);
	}

	return rv;
}

bool FlagSprite::init()
{
	genMesh();
	_program = ShaderCacheEx::instance()->shaderFor(ShaderType::Flag);
	GLuint program = _program->program();
	u_Center = glGetUniformLocation(program, "Center_worldspace");
	u_CameraRight = glGetUniformLocation(program, "CameraRight_worldspace");
	u_CameraUp = glGetUniformLocation(program, "CameraUp_worldspace");
	u_VP = glGetUniformLocation(program, "VP");
	u_ID = glGetUniformLocation(program, "u_ID");
	u_StartAngle = glGetUniformLocation(program, "uStartAngle");

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, _mesh.size() * sizeof(_mesh[0]), _mesh.data(), GL_STATIC_DRAW);
	_vbo = vbo;
	return true;
}

void FlagSprite::genMesh()
{
	int start = 0;
	genMeshSub(glm::vec2(-0.5f,-0.5f), glm::vec2(0.5,0.5), start);
}

// Left-bottom to right-top
void FlagSprite::genMeshSub(glm::vec2 p0, glm::vec2 p1, int dep)
{
	glm::vec2 p2(p1.x, p0.y);
	glm::vec2 p3(p0.x, p1.y);

	if (maxDepth_ == dep){
		_mesh.push_back(p0);
		_mesh.push_back(p2);
		_mesh.push_back(p3);

		_mesh.push_back(p3);
		_mesh.push_back(p2);
		_mesh.push_back(p1);
		return;
	}

	glm::vec2 X = (p0+p3) * 0.5f;
	glm::vec2 Y = (p0+p2) * 0.5f;
	glm::vec2 Z = (p2+p1) * 0.5f;
	glm::vec2 W = (p3+p1) * 0.5f;
	glm::vec2 S = (p0+p1) * 0.5f;

	genMeshSub(p0, S, dep + 1);
	genMeshSub(Y, Z,  dep + 1);
	genMeshSub(X, W,  dep + 1);
	genMeshSub(S, p1, dep + 1);
}


void FlagSprite::draw(const glm::mat4 &VP)
{
	DD::StateSaver _;
	glm::mat4 MVP = VP * _model;		// P * V * M

	glm::mat4 view = AppController::sharedInstance().getView();
	glm::mat4 projection = AppController::sharedInstance().getProjection();
	//glm::mat4 VP = projection * view;

	glActiveTexture(GL_TEXTURE0);
	CheckGL()
	glBindTexture(GL_TEXTURE_2D, tex_);
	CheckGL()
	glUseProgram(_program->program());
	CheckGL()

	glUniform2f(u_Center, 0, 0);
	glUniform3f(u_CameraRight, view[0][0], view[1][0], view[2][0]);
	glUniform3f(u_CameraUp, view[0][1], view[1][1], view[2][1]);
	glUniformMatrix4fv(u_VP, 1, GL_FALSE, &VP[0][0]);
	glUniform1f(u_StartAngle, uStartAngle_);
	glUniform1i(u_ID, 0);	// Sample 0


	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, _mesh.size());
	glDisableVertexAttribArray(0);

	CheckGL()
}
