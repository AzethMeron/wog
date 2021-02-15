#pragma once

#include "../erm_lua.h"

// Lua
int LuaUnloadCustomLOD(lua_State *L);
int LuaLoadCustomLOD(lua_State *L);
int LuaCalcObjects(lua_State *L);
int LuaFindNextObjects(lua_State *L);
int LuaDoesFileExist(lua_State *L);