

#include "LuaScriptor.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


static LuaScriptor* _instance = nullptr;

LuaScriptor::~LuaScriptor(){
	lua_close(_L);	
}

LuaScriptor::LuaScriptor(){
	_L = luaL_newstate();
	luaL_openlibs(_L);

	//Which gives our more info about the error
	const char *insertion = 
	"function __G__TRACKBACK__(msg)\n"
	"	local msg = debug.traceback(msg, 3);\n"
	"	print(msg)\n"
	"	return msg\n"
	"end\n";
	luaL_dostring(_L, insertion);
}

LuaScriptor* LuaScriptor::sharedInstance()
{
	if (!_instance){
		_instance = new LuaScriptor();
	}
	return _instance;
}

lua_State* LuaScriptor::getLuaState()
{
	return _L;
}