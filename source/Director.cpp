

#include "Director.h"
#include "Macros.h"
static EsDirector* _instance = nullptr;

EsDirector::EsDirector()
{
	_root = nullptr;
	//_light = 0;
}

EsDirector::~EsDirector()
{
	_root->destroy();
}

void EsDirector::init()
{
	_vao.make();
	
	//~
	glEnable(GL_DEPTH_TEST);
	CheckGL()
	glDepthFunc(GL_LESS);
	CheckGL()
	glEnable(GL_BLEND);
	CheckGL()
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	CheckGL()
	glEnable(GL_ALPHA_TEST);
	CheckGL()
}

void EsDirector::deinit()
{
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
	if(std::find(_light.begin(), _light.end(), light) == _light.end()){
		_light.push_back(light);
	}
}

int EsDirector::lightCount()const{
	return _light.size();
}

DDPoint EsDirector::getLightPosition(int index)
{
	if(index>=0&&index<_light.size()){
		return _light[index]->position();
	}
	return DDPoint(1,1,1);
}