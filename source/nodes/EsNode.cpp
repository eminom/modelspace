

#include "EsNode.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

EsNode::EsNode()
{
	_rotate = glm::mat4(1.0);
	_translate = glm::mat4(1.0);
	_scale = glm::mat4(1.0);
	_program = 0;	//
	_vbo = 0;
	_visible = true;
}

EsNode::~EsNode()
{
	if(_vbo){
		glDeleteBuffers(1, &_vbo);
		_vbo = 0;
	}
}

void EsNode::destroy()
{
	delete this;
}

void EsNode::addSub(EsNode *sub)
{
	_subs.push_back(sub);
}

void EsNode::render(const glm::mat4 &transform)
{
	if(_visible){
		draw(transform);
	}
	for(EsNode *one:_subs){
		one->render(transform);
	}
}

void EsNode::draw(const glm::mat4 &transform)
{

}

bool EsNode::init()
{
	return true;
}

void EsNode::setTransform(const glm::mat4 &trans)
{
	_translate = trans;
	synthesizeMatrix();
}

void EsNode::setTransform(float xf, float yf, float zf)
{
	_translate = glm::translate(glm::vec3(xf, yf, zf));
	synthesizeMatrix();
}

void EsNode::setRotate(float radians, float x, float y, float z)
{
	_rotate = glm::rotate(radians, glm::vec3(x,y,z));
	synthesizeMatrix();
}

void EsNode::setScale(float x)
{
	setScale(x,x,x);
}

void EsNode::setScale(float xs, float ys, float zs)
{
	if(xs<0){
		xs = _scale[0][0];
	}
	if(ys<0){
		ys = _scale[1][1];
	}
	if(zs<0){
		zs = _scale[2][2];
	}
	_scale = glm::scale(glm::vec3(xs, ys, zs));
	synthesizeMatrix();
}

void EsNode::synthesizeMatrix()
{
	//Do nothing before this. 
	_model = _translate * _rotate * _scale;
}


void EsNode::doUpdate(float dt)
{
	for(auto one:_subs){
		one->doUpdate(dt);
	}
	update(dt);
}

void EsNode::update(float dt)
{

}

void EsNode::setVisible(bool vis)
{
	_visible = vis;
}


void EsNode::setColor(float r, float g, float b, float a)
{

}