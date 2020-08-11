#pragma once
#include "../structs.h"

// Header info's
#define ERM_ERROR_HEADER LuaPushERMInfo(sp->Self.s,false)

// Log files
#define WOGERMLOG "woglogs/WOGERMLOG.TXT"
#define WOGLUALOG "woglogs/WOGLUALOG.TXT"
#define WOGCRASHDUMP "woglogs/WOGCRASHDUMP.DMP"
#define WOGCRASHLOG "woglogs/WOGCRASHLOG.TXT"

// Composite, back-end tools
void MakeErmVarNumDescription(char* destination, int length, VarNum& var);
void MakeErmArgDescription(char* destination, const int& length, _ToDo_* sp);
void MakeErmParamDescription(char* destination, const int& length, Mes *m, const int& Num);
void MakeErmErrorHeader(char* destination, const int& length, const char* info);
// Front-end tools
void MakeErmErrorMessage(char* destination, const int& length, _ToDo_* sp, Mes *m, const int& Num, const char* header_info); 
void UniversalErrorMessage(const char* message, const char* logfilename);
