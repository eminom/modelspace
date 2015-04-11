

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

#define AddS(k, vsh, fsh)\
	shaders_.insert(std::make_pair(ShaderType::k, ExShader::create("shaders/" vsh, "shaders/" fsh)));

void ShaderCacheEx::init()
{
	deinit();
	AddS(Simple, "SimpleVertexShader.vsh", "SimpleFragmentShader.fsh")
	AddS(Mimic, "MimicShader.vsh", "MimicShader.fsh")
	AddS(Diffuse, "Diffuse.vsh", "Diffuse.fsh")
	AddS(Wobbler, "Passthrough.vsh", "WobblyTexture.fsh")
	AddS(Particle, "Particle.vsh", "Particle.fsh")
	AddS(ParticleCube, "ParticleCube.vsh", "ParticleCube.fsh")
	AddS(ParticleTex, "ParticleTex.vsh", "ParticleTex.fsh")
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