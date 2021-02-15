#pragma once
#include <cstdlib>
#include <cstring>
#include "ErrorMess.h"
#include "lua_stdlib.h"
#include "../common.h"
#include "../erm.h"
#include "../lod.h"
#include "../service.h"

#define __FILENUM__ 34

int LuaCalcObjects(lua_State *L)
{
	int type = lua_tointeger(L,1);
	int subtype = lua_tointeger(L,2);
	lua_pushinteger(L,CalcObjects(type,subtype));
	return 1;
}

int LuaFindNextObjects(lua_State *L)
{
	int type = lua_tointeger(L,1);
	int subtype = lua_tointeger(L,2);
	int direction = lua_tointeger(L,3);
	int x = lua_tointeger(L,4);
	int y = lua_tointeger(L,5);
	int z = lua_tointeger(L,6);

	if(FindNextObjects(type,subtype,&x,&y,&z,direction)) { MError("LuaFindNextObjects: cannot find more objects."); }

	lua_pushinteger(L,x);
	lua_pushinteger(L,y);
	lua_pushinteger(L,z);
	return 3;
}

int LuaDoesFileExist(lua_State *L)
{
	char name[1024] = "";
	sprintf_s(name,1024,"%s",lua_tostring(L,1));
	int modifier = lua_tointeger(L,2);
	lua_pushinteger(L,DoesFileExist(name,modifier));
	return 1;
}

// exposing LOD support to Lua
// note: I don't understand how LODs work. Im just assuming all code regarding them works fine,
// and that kind=2 works aswell, despite it isn't mentioned in changelogs and has no documentation
int LuaLoadCustomLOD(lua_State *L)
{
	// Input
	int location = lua_tointeger(L,1);
	char name[1024] = "";
	sprintf_s(name,1024,"%s",lua_tostring(L,2));
	int kind = lua_tointeger(L,3);
	// Loading LOD
	int ind = Lod::LoadCustomLOD(location, name, kind);
	// Returning index
	lua_pushinteger(L,ind);
	return 1; // return informs LUA that X values on stack are returned values!
}

int LuaUnloadCustomLOD(lua_State *L)
{
	Lod::UnloadCustomLOD(lua_tointeger(L,1));
	return 0;
}