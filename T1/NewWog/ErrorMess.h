#pragma once
#include "../structs.h"
#include "../erm.h"

#define ERM_ERROR_HEADER LuaPushERMInfo(sp->Self.s,false)

// Composite, back-end tools
void MakeErmVarNumDescription(char* destination, int length, VarNum& var);
void MakeErmArgDescription(char* destination, const int& length, _ToDo_* sp);
void MakeErmParamDescription(char* destination, const int& length, Mes *m, const int& Num);
void MakeErmErrorHeader(char* destination, const int& length, const char* info);
// Front-end tools
void MakeErmErrorMessage(char* dst, const int& length, _ToDo_* sp, Mes *m, const int& Num);
void UniversalErrorMessage(const char* message, const char* logfilename);
