#pragma once
#include "../structs.h"

// Other, useful for debugging
#define ERR_BUFSIZE (4096)
#define MESSAGE_YES (30725)
#define MESSAGE_NO (30726)

// Macro, if defined, then Lua/ERM errors are managed by C/C++ code
// otherwise, error is passed to Lua
#define LERM_ERROR_IN_C
// Works only for Lua/ERM errors, others are managed by Lua anyway

// Log files
#define WOGERMLOG ("woglogs/WOGERMLOG.TXT")
#define WOGLUALOG ("woglogs/WOGLUALOG.TXT")
#define WOGCRASHDUMP ("woglogs/WOGCRASHDUMP.DMP")
#define WOGCRASHLOG ("woglogs/WOGCRASHLOG.TXT")
#define WOGVARLOG ("woglogs/WOGVARLOG.TXT")
#define ERMVARUSED ("woglogs/ERMVarsUsed.LOG")
#define UNCLOSEDSTACKLEVELS ("woglogs/UnclosedStackLevels.txt")

// Composite, back-end tools
void MakeErmVarNumDescription(char* destination, int length, VarNum& var);
void MakeErmArgDescription(char* destination, const int& length, _ToDo_* sp);
void MakeErmParamDescription(char* destination, const int& length, Mes *m, const int& Num);
void MakeErmErrorHeader(char* destination, const int& length, const char* info);
// Log
void WriteLog(const char* logfilename, const char* message, const bool oneLine = true);
// Front-end tools
void MakeErmErrorMessage(char* destination, const int& length, _ToDo_* sp, Mes *m, const int& Num, const char* header_info); 
int UniversalErrorMessage(const char* message, const char* logfilename = NULL, const int& mode = 1);
// Predefined error messages
void ErmSemanticError(_ToDo_* sp, Mes *m, const int& Num);
void ErmSyntaxError(Mes *m, const int& Ind);
void LuaErmError(_ToDo_* sp, Mes* m, int Num, char* err_mess);
// Old error messages
void _Error(int file,int line);
void _MError(int file,int line,char *Txt);
void _TError(int File,int Line,char *Txt);
void DumpMessage(char *txt,int offset);
void DumpERMVars(char *Text, bool NoLuaTraceback);
