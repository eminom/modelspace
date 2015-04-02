

#include "exports.h"
#include "LuaScriptor.h"
#include "Director.h"
#include "nodes/DDNodes.h"
#include "interop/lua_access/lua_access_macros.h"
#include "interop/lua_core/lua_core.h"
#include "lextension.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "EsNode.inc" 

static int getEsNodeRoot(lua_State *L)
{
	EsNodeWrapper *wrapper = createUserData<EsNodeWrapper>(L);
	wrapper->_ref = EsDirector::instance()->root();
	return 1;
}

static int setEsNodeLight(lua_State *L)
{
	EsNodeWrapper *wrapper = checkUserData<EsNodeWrapper>(L,1);
	if(LightProtocol *light = dynamic_cast<LightProtocol*>(wrapper->_ref)){
		EsDirector::instance()->setLight(light);
	}
	return 0;
}

static luaL_Reg entries[]={
	{"GetEsNodeRoot", getEsNodeRoot},
	{"SetLight", setEsNodeLight},
	//{"CreateLightNode", createLightNode},
	{NULL, NULL},
};

void doExports()
{
	loadEsNodeTmpIndex();
	loadBlenderNodeTmpIndex();
	loadBlenderNormalTmpIndex();
	loadAxisNodeTmpIndex();
	loadLightNodeTmpIndex();
	loadFieldNodeTmpIndex();
	loadMimicNodeTmpIndex();
	loadCubeNodeTmpIndex();

	load_ddStatics();
	_DeclareState()
	luaL_register(L, "ddcore", entries);
}