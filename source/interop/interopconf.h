


#ifndef _INTEROP_CONF_DEF__
#define _INTEROP_CONF_DEF__

#include "interop/class/experimental.h"
#include "interop/class/exports.h"
#include "interop/lua_core/lua_core.h"

#define _InitLuaVM(S)\
	do{\
		LuaCore::doExports(S);\
		LuaCore::injectLuaCore(S);\
		LuaCore::injectExperimental(S);\
	}while(0)

#endif