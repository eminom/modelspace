
#include "lua_access.h"




ObjContainer::ObjContainer(lua_State *L):
	_ref(LUA_REFNIL),
	_objCount(0) ,
	L_(L) {
	_ref = ljMakeTable(L_);
}

ObjContainer::~ObjContainer(){
	ljReleaseObj(L_, _ref);
}

void ObjContainer::clear(){
	ljReleaseObj(L_, _ref);
	_objCount = 0;	//From Zero;
	assert( LUA_REFNIL == _ref);
	_ref = ljMakeTable(L_);	//Another table;
}

bool ObjContainer::addObject(int obj) {
	lua_State *L = L_;
	const int top = lua_gettop(L);
	lua_rawgeti(L, LUA_REGISTRYINDEX, _ref);
	assert(lua_istable(L,-1)) ;
	lua_rawgeti(L,LUA_REGISTRYINDEX, obj);
	if (!lua_istable(L,-1)){
		assert(false);
		lua_pop(L, 2);
		return false;
	}
	int indexKey = _objCount + 1;	//The last one
	lua_rawseti(L, -2, indexKey);
	lua_pop(L, 1);
	assert( lua_gettop(L) == top);
	_objCount++;
	return true;
}

int ObjContainer::tableRef(){
	return _ref;
}