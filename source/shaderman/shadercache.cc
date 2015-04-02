

#include "shadercache.h"
#include "exshader.h"
#include <cassert>

static ShaderCacheEx* inst_ = 0;

ShaderCacheEx::ShaderCacheEx()
{

}

ShaderCacheEx* ShaderCacheEx::instance()
{
	if(!inst_){
		inst_ = new ShaderCacheEx;
		inst_->init();
	}
	return inst_;
}


void ShaderCacheEx::init()
{
	deinit();
	shaders_.insert(std::make_pair(ShaderType::Simple
		, ExShader::create("shaders/SimpleVertexShader.vsh", "shaders/SimpleFragmentShader.fsh")));
	shaders_.insert(std::make_pair(ShaderType::Mimic
		, ExShader::create("shaders/MimicShader.vsh", "shaders/MimicShader.fsh")));
	shaders_.insert(std::make_pair(ShaderType::Diffuse
		, ExShader::create("shaders/Diffuse.vsh", "shaders/Diffuse.fsh")));
}

ExShader* ShaderCacheEx::shaderFor(ShaderType t)
{
	auto it = shaders_.find(t);
	if(it!=shaders_.end()){
		return it->second;
	}
	assert(0);
	return 0;
}

void ShaderCacheEx::deinit()
{
	for(auto each:shaders_){
		delete each.second;
	}
	shaders_.clear();
}