
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


void onDo3(unsigned a, int b, char c)
{
	printf("onDo3 %u, %d, %c\n", a,b,c);
}

void onDo2(int a, int b)
{
	printf("onDo2(%d,%d)\n", a,b);
}

void onDo1(int a)
{
	printf("onDo1 : %d\n", a);
}



DD::Signal<void(int)> s;
DD::SlotHandle one;

void s1(int a)
{
	printf("s1:%d\n",a);
}

void s2(int a)
{
	printf("s2:%d\n", a);
}

void s3(int a)
{
	printf("s3:%d\n", a);
	if(one){
		s.disconnect(one);
		one = nullptr;
	}
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
		s.connect(std::bind(&s1,_1), 1);
		one = s.connect(std::bind(&s2,_1));
		s.connect(std::bind(&s3,_1));
		printf("############################\n\n");
		s.trigger(101);
		printf("############################\n\n");
		s.trigger(101);
		printf("############################\n\n");
	}while(0);

	do{
		DD::Signal<void(int)> s;
		s.connect(std::bind(&onDo1,_1));
		s.trigger(200);
	}while(0);

	do{
		DD::Signal<void(int)> s;
		s.connect(std::bind(&onDo2, 200,_1));
		s.trigger(250);
	}while(0);

	do 
	{
		DD::Signal<void(unsigned,int,char)> s;
		s.connect(std::bind(&onDo3, _1,_2,_3));
		s.trigger(255, 100, 'c');
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
