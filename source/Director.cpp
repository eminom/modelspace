

#include "Director.h"

static EsDirector* _instance = nullptr;

EsDirector::EsDirector()
{
	_vao = 0;
	_root = nullptr;
	_light = 0;
}

EsDirector::~EsDirector()
{
	_root->destroy();
}

void EsDirector::init()
{
	assert( !_vao);
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	//~
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
}

void EsDirector::deinit()
{
	assert ( !_vao);
	glDeleteVertexArrays(1, &_vao);
	_vao = 0;
}

EsDirector* EsDirector::instance()
{
	if(!_instance)
	{
		_instance = new EsDirector;
		_instance->init();
	}
	return _instance;
}

void EsDirector::purge()
{
	if(_instance)
	{
		_instance->deinit();
		delete _instance;
		_instance = nullptr;
	}
}

void EsDirector::render(const glm::mat4 &matrix)
{
	_root->render(matrix);
}

void EsDirector::updateNode(float dt)
{
	_root->doUpdate(dt);
}

void EsDirector::setRoot(EsNode *root)
{
	assert(!_root);
	_root = root;
}

EsNode* EsDirector::root()const
{
	return _root;
}

void EsDirector::setLight(LightProtocol *light)
{
	_light = light;
}

DDPoint EsDirector::getLightPosition()
{
	if(_light){
		return _light->position();
	}
	return DDPoint(1,1,1);
}