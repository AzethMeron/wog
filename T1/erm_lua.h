#pragma once


#ifndef __ERM_LUA__
#define __ERM_LUA__

#include "structs.h"
//#include "luaW_debug.h"
extern "C"
{
	#include "lua.h"
}
//#include "lualib.h"
//#include "lauxlib.h"

extern char *LuaErrorString; // const
extern void LuaLastError(const char *error);
extern lua_State *Lua;

int LuaPCall(int nArgs, int nResults);

int __stdcall OnDialogAction(Dword body, Dword cmd);
void __fastcall HookModalDialog(int** dlg);

void InitLua();
//void RunLuaString(const char *chunk, const char *name);
void CallLuaTrigger(int index);

#define LUACALL_LUA Lua
#include "LuaCall.h"
int LuaCallStart(const char *name);
int LuaCallEnd();

void LuaInternalConst(const char *name, int value);

const char* LuaPushERMInfo(char *erm, bool OneLine = true);

static int LuaError(char *text, int level);
static int LuaError(char *text, int level, _ToDo_* sp, Mes* m, int Num);
//void LuaErmError(_ToDo_* sp, Mes* m, int Num, char* err_mess);

#endif