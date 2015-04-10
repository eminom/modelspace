




#include "BlenderParticle.h"
#include "model/ObjectModel.h"
#include <glm/glm.hpp>
#include "Director.h"

BlenderParticle::~BlenderParticle()
{
	delete [] positionInfo_;
	delete [] colorInfo_;
}

BlenderParticle::BlenderParticle(int max, float scale)
	:particleMax_(max)
	,fScale_(scale)
{
	positionInfo_ = nullptr;
	colorInfo_ = nullptr;

}

BlenderParticle* BlenderParticle::create(const char *path, int max, float fScale,float x, float y, float z)
{
	auto rv = BlenderNormal::create<BlenderParticle>(path, true, max, fScale);
	//rv->bornPos_ = DDPoint(x,y,z);
	rv->setTransform(x,y,z);  //either is ok.
	return rv;
}

bool BlenderParticle::initComplex(const char *path)
{
	_program = ShaderCacheEx::instance()->shaderFor(ShaderType::ParticleCube);
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

	// The particle scene
	// Reuse
	GLuint color = 0;
	glGenBuffers(1, &color);
	glBindBuffer(GL_ARRAY_BUFFER, color);
	glBufferData(GL_ARRAY_BUFFER, particleMax_ * 4 * sizeof(GLfloat), 0, GL_STREAM_DRAW);
	_colorVBO.take(color);

	GLuint posfix = 0;
	glGenBuffers(1, &posfix);
	glBindBuffer(GL_ARRAY_BUFFER, posfix);
	glBufferData(GL_ARRAY_BUFFER, particleMax_ * 4 * sizeof(GLfloat), 0, GL_STREAM_DRAW);
	posfix_.take(posfix);

	assert(!positionInfo_);
	assert(!colorInfo_);

	positionInfo_ = new GLfloat[particleMax_ * 4];
	colorInfo_ = new GLfloat[particleMax_ * 4];
	return true;
}

void BlenderParticle::update(float dt)
{
	int newCount = int(dt*1000);
	if(newCount > particleMax_ - nodes_.size()){
		newCount = particleMax_ - nodes_.size();
	}
	// generates news
	for(int i=0;i<newCount;++i){
		BNode *node = pool_.alloc();
		node->color.r = (rand()%256) / 255.0f;
		node->color.g = (rand()%256) / 255.0f;
		node->color.b = (rand()%256) / 255.0f;
		node->color.a = (rand()%256) / 255.0f;

		glm::vec4 maindir = glm::vec4(0, 1.0f, 0, 0);
		float spread = 0.15f;
		glm::vec4 dir = glm::vec4(
			(rand()%21 / 10.0f - 1) * 2,
			(rand()%21 / 10.0f - 1) * 2,
			(rand()%21 / 10.0f - 1) * 2,
			0
		);
		node->speed = maindir + dir * spread;
		node->posfix = glm::vec4(bornPos_, fScale_);	//`scale all the same
		node->life = rand() % 15 + 1;
		nodes_.push_back(node);
	}

	//~step
	for(auto &one:nodes_){
		one->speed = one->speed + glm::vec4(0, -9.8f, 0, 0) * 0.1f * dt;
		one->posfix += one->speed * dt;
		one->life -= dt;
	}
	
	//~
	for(auto it=nodes_.begin();it!=nodes_.end();){
		BNode *one = *it;
		if(one->life <= 0){
			pool_.free(one);
			nodes_.erase(it++);
		} else {
			++it;
		}
	}

	int pc = 0;
	for(auto &one:nodes_){
		positionInfo_[pc*4 + 0] = one->posfix.x;
		positionInfo_[pc*4 + 1] = one->posfix.y;
		positionInfo_[pc*4 + 2] = one->posfix.z;
		positionInfo_[pc*4 + 3] = one->posfix.w;

		colorInfo_[pc*4 + 0] = one->color.r;
		colorInfo_[pc*4 + 1] = one->color.g;
		colorInfo_[pc*4 + 2] = one->color.b;
		colorInfo_[pc*4 + 3] = one->color.a;
		++pc;
	}

	//printf("Now up to %d\n", pc);

	glBindBuffer(GL_ARRAY_BUFFER, _colorVBO);
	glBufferData(GL_ARRAY_BUFFER, particleMax_ * 4 * sizeof(GLfloat), 0, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particleMax_ * 4 * sizeof(GLfloat), colorInfo_);

	glBindBuffer(GL_ARRAY_BUFFER, posfix_);
	glBufferData(GL_ARRAY_BUFFER, particleMax_ * 4 * sizeof(GLfloat), 0, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particleMax_ * 4 * sizeof(GLfloat), positionInfo_);
}

void BlenderParticle::drawComplex(const glm::mat4 &mat)
{
	glm::mat4 mvp = mat * _model;
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(_model)));
	
	_program->use();
	glUniformMatrix3fv(_normalMatrixSlot, 1, GL_FALSE, &normalMatrix[0][0]);
	setLightUniforms();
	glUniformMatrix4fv(_mvpSlot, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(_modelSlot, 1, GL_FALSE, &_model[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _normalVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, posfix_);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, _colorVBO);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);

	glDrawArraysInstanced(GL_TRIANGLES, 0, _vertex.size(), nodes_.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}
