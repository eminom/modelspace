
#include <stdio.h>
#include <stdlib.h>


extern "C"{
#include "lauxlib.h"
#include "lualib.h"
#include "lua.h"

}

int callFunc(lua_State *L)
{
	lua_getglobal(L, "f");
	lua_pushinteger(L,3);
	lua_pushinteger(L,7);
	if(lua_pcall(L,2,1,0)!=0){
		printf("something not right.\n");
		//abort
	}

	if(!lua_isnumber(L,-1)){
		printf("not a number!\n");
		//abort
	}

	int z = lua_tonumber(L,-1);
	lua_pop(L,1);
	return z;
}

int mainZ()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	char *buf = "function f(x,y) return x^2 + y; end";
	int error = luaL_loadstring(L,buf);
	lua_pcall(L,0,0,0);
	int z = callFunc(L);

	lua_close(L);
	return 0;
}
