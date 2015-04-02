

#ifndef _LUA_SCRIPTOR__DEF__
#define _LUA_SCRIPTOR__DEF__

struct lua_State;

class LuaScriptor
{
private:
	LuaScriptor();
	~LuaScriptor();

public:
	static LuaScriptor* sharedInstance();
	static void purge();
	lua_State* getLuaState();

private:
	lua_State *_L;
};


#endif