#define __FILENUM__ 32
#include "ErrorMess.h"
#include <stddef.h>
#include <stdio.h>
#include "../common.h"
#include "../txtfile.h"
#include "../b1.h"
#include <algorithm>
#define GameLoaded  MainWindow

/****************************************************************************************/
// Tools, "composite"

// destination - string to store output
// length - size of destination string
// Created for arguments and parameters of receiver. Doesn't support 'dependencies flags' 
// Oh god this function is TRASH! Please smb remake this
// No support for constant ^^ strings
void MakeErmVarNumDescription(char* destination, int length, VarNum& var)
{
	sprintf_s(destination, length, "GetVarNumDescription error: report this");
	const char* Type[] = {
		"constant", // 0
		"flag", // 1
		"fast-var", // 2
		"v", // 3
		"w", // 4
		"x", // 5
		"y", // 6
		"z", // 7
		"e", // 8
		"unrecognised", // 9
		"unrecognised", // 10
	};
	switch(var.Check)
	{
	case 0: /* set syntax */ {
		switch(var.Type)
		{
		case 0: { if(var.Num == 0) sprintf_s(destination,length,"{%s} value %d, or constant {string} (^string^)", Type[var.Type], var.Num); else sprintf_s(destination,length,"{%s} value %d", Type[var.Type], var.Num); } break;
		case 2: { sprintf_s(destination,length,"{%s}, value %d", Type[var.Type], GetVarVal(&var)); } break;
		case 7: { if(var.IType == 0) /*Constant-indexed zvar*/ { sprintf_s(destination,length,"{%s%d}, strings aren't displayed here", Type[var.Type], var.Num); } 
				 else /*Indexed with another variable*/ { sprintf_s(destination,length,"{%s%s%d}, index %s%d=%d, strings aren't displayed here", Type[var.Type], Type[var.IType], var.Num, Type[var.IType], var.Num, GetVarIndex(&var,true)); } } break;
		default: { if(var.IType == 0) /*Constant-indexed variable*/ { sprintf_s(destination,length,"{%s%d}, value {%d}", Type[var.Type], var.Num, GetVarVal(&var)); } 
				 else /*Indexed with another variable*/ { sprintf_s(destination,length,"{%s%s%d}, index %s%d=%d, value {%d}", Type[var.Type], Type[var.IType], var.Num, Type[var.IType], var.Num, GetVarIndex(&var,true), GetVarVal(&var)); } } break;
		}
	} break; 
	case 1: /* get syntax */ {
		switch(var.Type)
		{
		case 0: { sprintf_s(destination,length,"error: you can't use get-syntax with {%s} value", Type[var.Type]); } break;
		case 2: { sprintf_s(destination,length,"get-syntax, {%s}, value {undefined}", Type[var.Type]); } break;
		default: { if(var.IType == 0) /*Constant-indexed variable*/ { sprintf_s(destination,length,"get-syntax, {%s%d}, value {undefined}", Type[var.Type], var.Num); } 
				 else /*Indexed with another variable*/ { sprintf_s(destination,length,"get-syntax, {%s%s%d}, index %s%d=%d, value {undefined}", Type[var.Type], Type[var.IType], var.Num, Type[var.IType], var.Num, GetVarIndex(&var,true)); } } break;
		}
	} break; 
	default: { sprintf_s(destination,length,"unrecognised"); } break; 
	}
}

void MakeErmArgDescription(char* destination, const int& length, _ToDo_* sp)
{
	char* message = new char[length];
	char* buffer = new char[length];
	sprintf_s(buffer,length,"Arguments (there's always one)\n"); 
	std::swap(message,buffer);
	for(int i = 0; i < sp->ParSet; ++i)
	{
		char arg_description[256];
		MakeErmVarNumDescription(arg_description,256,sp->Par[i]);
		sprintf_s(buffer,length,"%sArg %d: %s\n", message, i+1, arg_description);
		std::swap(message,buffer);
	}
	sprintf_s(destination,length,"%s",message);
	delete message;
	delete buffer;
}

void MakeErmParamDescription(char* destination, const int& length, Mes *m, const int& Num)
{
	char* message = new char[length];
	char* buffer = new char[length];
	sprintf_s(buffer,length,"Parameters (if none, there is syntax error in params)\n"); 
	std::swap(message,buffer);
	for(int i = 0; i < Num; ++i)
	{
		char param_description[256];
		MakeErmVarNumDescription(param_description,256,m->VarI[i]);
		sprintf_s(buffer,length,"%sParam %d: %s\n", message, i+1, param_description);
		std::swap(message,buffer);
	}
	sprintf_s(destination,length,"%s",message);
	delete message;
	delete buffer;
}

void MakeErmErrorHeader(char* destination, const int& length, const char* info = "ErrorMess: info not initialised")
{
	sprintf_s(destination, length, "%s\n\n%s", ITxt(24,0,&Strings), info);
}

/****************************************************************************************/
// High-end tools

void MakeErmErrorMessage(char* destination, const int& length, _ToDo_* sp, Mes *m, const int& Num)
{
	const int bufsize = 4096;
	char header[bufsize];
	MakeErmErrorHeader(header,bufsize,ERM_ERROR_HEADER);
	char arguments[bufsize];
	MakeErmArgDescription(arguments,bufsize,sp);
	char parameters[bufsize];
	MakeErmParamDescription(parameters,bufsize,m,Num);
	sprintf_s(destination,length,"%s\n%s\n%s",header,arguments,parameters);
}

void UniversalErrorMessage(const char* message, const char* logfilename)
{

}