


#ifndef _SHADER_CACHE_EX_DEF__
#define _SHADER_CACHE_EX_DEF__

#include <string>
#include <map>
#include <utility>
#include "exshader.h"

enum class ShaderType{
	Simple,
	Diffuse,
	Mimic
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
