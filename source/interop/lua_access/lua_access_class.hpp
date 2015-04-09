
#ifndef _LUA_ACCESS_CLASS_DEF__
#define _LUA_ACCESS_CLASS_DEF__

//incuded by lua_access.h
#include <cassert>

class FuncRef
{
public:
	FuncRef::FuncRef()
		:_ref(LUA_REFNIL)
		,_refCounter(nullptr)
	{
	}

	FuncRef(const char *name)
		:_ref(LUA_REFNIL)
		,_refCounter(nullptr)
	{
		loadFunc(name);
	}

	~FuncRef() 
	{
		deinit();
	}
	
	FuncRef(const FuncRef&rhs)
		:_ref(LUA_REFNIL)
		,_refCounter(nullptr){
		copy(rhs);
	}

	FuncRef& operator=(const FuncRef &rhs){
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

	void deinit()
	{
		if(_refCounter){
			--(*_refCounter);
			if(0 == *_refCounter){
				_DeclareState()
				luaL_unref(L, LUA_REGISTRYINDEX, _ref);
			}
		}
		//detached
		_ref = LUA_REFNIL;
		_refCounter = nullptr;
	}

	FuncRef& copy(const FuncRef &rhs)
	{
		deinit();
		if( rhs._refCounter){
			_ref = rhs._ref;
			_refCounter = rhs._refCounter;
			++(*_refCounter);
		}
		return *this;
	}

	bool require(const char *path)
	{
		deinit();
		_DeclareState()
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

private:
	void take(int ref)
	{
		assert(LUA_REFNIL == _ref);
		assert( ! _refCounter );
		assert( ref != LUA_REFNIL);
		_ref = ref;
		_refCounter = new int(1);
	}

public:
	void loadFunc(const char *name)
	{
		assert( LUA_REFNIL == _ref );
		take(ljLoadFuncHandle(name));
	}

	int createObj()
	{
		assert( LUA_REFNIL != _ref);
		return ljCreateTableFromFuncRef(_ref, 1, [](lua_State *L, int n)->int{return lua_istable(L,n);});
	}

private:
	int _ref;
	int *_refCounter;
};


#endif