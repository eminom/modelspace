


#ifndef _SHADER_CACHE_EX_DEF__
#define _SHADER_CACHE_EX_DEF__

#include <string>
#include <map>
#include <utility>
#include "exshader.h"

//If you find compiling issue on Xcode
//You should check your project settings first: The compilier specification(C++11)
enum class ShaderType{
	Simple,
	Diffuse,
	Mimic,
	Wobbler,
	Particle,
	ParticleCube,
	ParticleTex,
};

class ShaderCacheEx
{
protected:
	ShaderCacheEx();

public:
	static ShaderCacheEx* instance();
	ExShader* shaderFor(ShaderType);

private:
	void init();
	void deinit();

protected:
	std::map<ShaderType, ExShader*> shaders_;
};


#endif
