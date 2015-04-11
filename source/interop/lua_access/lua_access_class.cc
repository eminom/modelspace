
#include "lua_access.h"

FuncRef::FuncRef(lua_State *L)
	:_ref(LUA_REFNIL)
	,_refCounter(nullptr)
	,L_(L)
{
}

FuncRef::FuncRef(const char *name, lua_State *L)
	:_ref(LUA_REFNIL)
	,_refCounter(nullptr)
	,L_(L)
{
	loadFunc(name);
}

FuncRef::~FuncRef() 
{
	deinit();
}
	
FuncRef::FuncRef(const FuncRef&rhs)
	:_ref(LUA_REFNIL)
	,_refCounter(nullptr)
	,L_(nullptr) {
	copy(rhs);
}

FuncRef& FuncRef::operator=(const FuncRef &rhs){
	return copy(rhs);
}

//FuncRef::FuncRef(const FuncRef&rhs)
//	:_ref(LUA_REFNIL){
//	_ref = ljDuplicateObj(rhs._ref);
//}
//
//FuncRef& FuncRef::operator=(const FuncRef &rhs){
//	ljReleaseObj(_ref);
//	_ref = ljDuplicateObj(rhs._ref);
//	return *this;
//}

void FuncRef::deinit()
{
	if(_refCounter){
		--(*_refCounter);
		if(0 == *_refCounter){
			luaL_unref(L_, LUA_REGISTRYINDEX, _ref);
			delete _refCounter;
		}
	}
	//detached
	_ref = LUA_REFNIL;
	_refCounter = nullptr;
}

FuncRef& FuncRef::copy(const FuncRef &rhs)
{
	deinit();
	if( rhs._refCounter){
		_ref = rhs._ref;
		_refCounter = rhs._refCounter;
		L_ = rhs.L_;	//~ pure pointer.
		++(*_refCounter);
	}
	return *this;
}

bool FuncRef::require(const char *path)
{
	deinit();
	lua_State *L = L_;
	const int top = lua_gettop(L);
	int traceback = 0;
	lua_getglobal(L, _GTrackBack);
	if(lua_isfunction(L, -1)){
		traceback = -2;
	}
	luaL_loadfile(L, path);
	if( lua_pcall(L, 0, 1, traceback)){
		lua_pop(L, 2); // Pop the error message. The top is generated by the tracker. And pop the "traceback"
		assert(top==lua_gettop(L));
		fprintf(stderr, "Error loading from %s\n", path);
		return false;
	}
	if(!lua_isfunction(L, -1)){
		fprintf(stderr,"Expecting a function from %s\n", path);
		lua_pop(L, 2);
		assert(top == lua_gettop(L));
		return false;
	}
	take(luaL_ref(L, LUA_REGISTRYINDEX));
	lua_pop(L, 1);
	assert( top == lua_gettop(L));
	return true;
}

void FuncRef::take(int ref)
{
	assert(LUA_REFNIL == _ref);
	assert( ! _refCounter );
	assert( ref != LUA_REFNIL);
	_ref = ref;
	_refCounter = new int(1);
}

void FuncRef::loadFunc(const char *name)
{
	assert( LUA_REFNIL == _ref );
	take(ljLoadFuncHandle(L_, name));
}

int FuncRef::createObj()
{
	assert( LUA_REFNIL != _ref);
	return ljCreateTableFromFuncRef(L_, _ref, 1, [](lua_State *L, int n)->int{return lua_istable(L,n);});
}

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