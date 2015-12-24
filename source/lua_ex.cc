

#include "lua_ex.h"

extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "luasocket/luasocket.h"
//#include "luasocket/luasocket_scripts.h"
#include "luasocket/mime.h"
}

static luaL_Reg luax_exts[] = {
    {"socket.core", luaopen_socket_core},
    {"mime.core", luaopen_mime_core},
    {NULL, NULL}
};

void luaopen_ex(lua_State *L)
{
    // load extensions
    luaL_Reg* lib = luax_exts;
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    for (; lib->func; lib++)
    {
        lua_pushcfunction(L, lib->func);
        lua_setfield(L, -2, lib->name);
    }
    lua_pop(L, 2);
    
    //luaopen_luasocket_scripts(L);
}
