
#include <stdio.h>
#include <stdlib.h>


extern "C"{
#include "lauxlib.h"
#include "lualib.h"
#include "lua.h"

}

#include <functional>
#include "utils/signalsinc.hpp"

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


void onDo2(int a, int b)
{
	printf("onDo2(%d,%d)\n", a,b);
}

void onDo1(int a)
{
	printf("onDo1 : %d\n", a);
}

void test2()
{
	//DD::Signal<void(int,int)> s;
	
	//auto k = std::bind(&onDo,_1,_2);
	////k(1,2);
	//s.connect(std::bind(&onDo,_1,_2));
	//s.trigger(20,30);

	using namespace std::placeholders;

	do{
		DD::Signal<void(int)> s;
		s.connect(std::bind(&onDo1,_1));
		s.trigger(200);
	}while(0);

	do{
		DD::Signal<void(int,int)> s;
		s.connect(std::bind(&onDo2, _1,_2));
		s.trigger(201,302);
	}while(0);
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
