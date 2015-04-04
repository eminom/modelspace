
#include "lua_access.h"
#include "refobj.h"

RefObject::RefObject()
	:_ref(LUA_REFNIL),
	_ref_count(nullptr){
}

RefObject::RefObject(const char *name)
	:_ref(LUA_REFNIL),
	_ref_count(nullptr)
{
	loadRef(name);
}

RefObject::RefObject(const RefObject& rhs)
	:_ref(LUA_REFNIL),
	_ref_count(nullptr)
{
	copy(rhs);
}

RefObject& RefObject::copy(const RefObject &rhs)
{
	deinit();
	if(rhs._ref != LUA_REFNIL){
		_ref = rhs._ref;
		_ref_count = rhs._ref_count;
		++(*_ref_count);
	}
	return *this;
}

RefObject& RefObject::operator=(const RefObject&rhs)
{
	return copy(rhs);
}

RefObject::~RefObject(){
	deinit();
}

void RefObject::deinit(){
	if(_ref!=LUA_REFNIL){
		--(*_ref_count);
		if(0==*_ref_count){
			ljReleaseObj(_ref);
			delete _ref_count;
			_ref_count = 0;
		}
	}
}

void RefObject::take(int refIndex){
	assert(LUA_REFNIL == _ref);
	assert( ! _ref_count);
	assert( LUA_REFNIL != refIndex);
	_ref = refIndex;
	_ref_count = new int(1);
}


void RefObject::loadRef(const char *name) {
	deinit();	//`!
	take(ljLoadObj(name));
}

bool RefObject::require(const char *path){
	deinit();
	_DeclareState()
	int traceback = 0;
	lua_getglobal(L, _GTrackBack);
	if( lua_isfunction(L, -1)){
		traceback = -2;
	}
	luaL_loadfile(L, path);
	if( lua_pcall(L, 0, 1, traceback) ){
		lua_pop(L, 2);	/// skip the error message and the tracker
		assert( top == lua_gettop(L) );
		fprintf(stderr, "Error executing script %s\n", path);
		return false;
	}
	if(!lua_istable(L, -1)){
		fprintf(stderr, "I need a table for this\n");
		lua_pop(L, 2); //both result(1) and the tracker
		assert( top == lua_gettop(L));
		return false;
	}
	take(luaL_ref(L, LUA_REGISTRYINDEX));
	lua_pop(L, 1);
	assert( top == lua_gettop(L));
}

void RefObject::loadFromFunc(const char *name){
	deinit();
	_DeclareState()
	int traceback = 0;
	lua_getglobal(L, _GTrackBack);
	if(lua_isfunction(L, -1)){
		traceback = -2;
	}
	lua_getglobal(L, name);
	if(!lua_isfunction(L, -1)){
		fprintf(stderr, "No a function for %s\n", name);
		lua_pop(L, 2);
		assert( top == lua_gettop(L));
		assert(0);
		return;
	}
	if( lua_pcall(L, 0, 1, traceback)){
		fprintf(stderr, "Error running %s\n", name);
		lua_pop(L, 2);
		assert( top == lua_gettop(L));
		assert(0);
		return;
	}
	take(luaL_ref(L, LUA_REGISTRYINDEX));
	lua_pop(L, 1);
	assert( top == lua_gettop(L));
	assert( _ref != LUA_REFNIL);
}

int RefObject::tableRef()const {
	return _ref;
}

void RefObject::createTableRef() {
	//~Do nothing at all.
}
