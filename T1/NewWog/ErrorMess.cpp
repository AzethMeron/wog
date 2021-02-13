#define __FILENUM__ 32
#include <stddef.h>
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <windows.h>

#include "ErrorMess.h"
#include "../erm.h"
#include "../common.h"
#include "../txtfile.h"
#include "../b1.h"
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
		case 0: { if(var.Num == 0) sprintf_s(destination,length,"{%s} value %d, or constant {string} (^^)", Type[var.Type], var.Num); else sprintf_s(destination,length,"{%s} value %d", Type[var.Type], var.Num); } break;
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
	for(int i = 0; i < length; ++i) { message[i] = '\0'; buffer[i] = '\0'; }
	sprintf_s(buffer,length,"Arguments (there's always one)\n"); 
	std::swap(message,buffer);
	for(int i = 0; i < sp->ParSet; ++i)
	{
		char arg_description[256] ="";
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
	for(int i = 0; i < length; ++i) { message[i] = '\0'; buffer[i] = '\0'; }
	sprintf_s(buffer,length,"Parameters (if none, syntax error occured)\n"); 
	std::swap(message,buffer);
	for(int i = 0; i < Num; ++i)
	{
		char param_description[256]="";
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
	sprintf_s(destination, length, "%s\n\n%s\n", ITxt(24,0,&Strings), info);
}

// parse lua traceback to more readable form
// destination string must be longer than traceback
void ParseLuaTraceback(char* destination, int Length, const char* traceback, int TraceLength)
{
	bool check = false;
	int j = 0;
	for(int i = 0; i < TraceLength; ++i)
	{
		if(j >= Length) return;
		destination[j] = traceback[i];
		if(traceback[i] == ' ' && check) 
		{
			++j;
			if(j >= Length) return;
			destination[j] = '\n';
		}
		check = false;
		if(traceback[i] == ':') check = true;
		++j;
	}
}

/****************************************************************************************/
// Logs

void WriteLog(const char* logfilename, const char* message, const bool oneLine)
{
	FILE* file = fopen(logfilename,"at");
	char final_message[ERR_BUFSIZE]="";
	time_t ltime;
	time(&ltime);
	if(file != NULL)
	{
		size_t size = strlen(message);
		sprintf_s(final_message,ERR_BUFSIZE,"%s	%s\n",asctime(gmtime( &ltime )),message);
		if(oneLine) { for(int i = 0; i < size; ++i) { if(final_message[i] == '\n') final_message[i] = '	'; } }
		fprintf(file, "%s", final_message);
		fclose(file);
	}
	else
	{
		sprintf_s(final_message,ERR_BUFSIZE,"Failed to open {log} file\n%s\n\nThis {may} mean that there is no {woglogs} folder in Heroes 3 main directory",logfilename);
		UniversalErrorMessage(final_message);
	}
}

/****************************************************************************************/
// High-end tools

void MakeErmErrorMessage(char* destination, const int& length, _ToDo_* sp, Mes *m, const int& Num, const char* header_info)
{
	STARTNA(__LINE__, 0)
	char header[ERR_BUFSIZE] = "";
	MakeErmErrorHeader(header,ERR_BUFSIZE,header_info);
	char arguments[ERR_BUFSIZE] = "";
	MakeErmArgDescription(arguments,ERR_BUFSIZE,sp);
	char parameters[ERR_BUFSIZE] = "";
	MakeErmParamDescription(parameters,ERR_BUFSIZE,m,Num);
	sprintf_s(destination,length,"%s\n%s\n%s",header,arguments,parameters);
	RETURNV
}

int UniversalErrorMessage(const char* message, const char* logfilename, const int& mode)
{
	STARTNA(__LINE__, 0)
	if(logfilename!=NULL) { WriteLog(logfilename,message,true); }
	if (GameLoaded)
	{
		if(PL_ERMErrDis!=1) RETURN(Message(message, mode));
	}
	else
	{
		MessageBox(0, message, "Error", 0);
	}
	RETURN(0)
}

/****************************************************************************************/
// Predefined error messages

// Error message for ERM
// Expanded by Jakub Grzana. Work in progress
void ErmSemanticError(_ToDo_* sp, Mes *m, const int& Num) // Marker
{
	STARTNA(__LINE__,&m->m.s[m->i])
	char last = m->m.s[m->m.l];
	m->m.s[m->m.l] = 0;

	char message[ERR_BUFSIZE]= "";
	MakeErmErrorMessage(message,ERR_BUFSIZE,sp,m,Num,LuaPushERMInfo(sp->Self.s,false));
	UniversalErrorMessage(message,WOGERMLOG);

	// ???
	lua_pop(Lua, 1);
	m->m.s[m->m.l] = last;
	++m->i;

	RETURNV
}

void ErmSyntaxError(Mes *M, const int& Ind)
{
	char err_mess[ERR_BUFSIZE] = "";
	sprintf_s(err_mess,ERR_BUFSIZE,"%s\n\n%s", ITxt(26,0,&Strings), LuaPushERMInfo(&M->m.s[Ind], false));
	lua_pop(Lua, 1);
	UniversalErrorMessage(err_mess,WOGERMLOG);
}

void LuaErmError(_ToDo_* sp, Mes* m, int Num, char* err_mess)
{
	char message[ERR_BUFSIZE] = "";
	
	char traceback[ERR_BUFSIZE] = "";
	LuaCallStart("traceback");
	LuaPCall(0, 1);
	ParseLuaTraceback(traceback,ERR_BUFSIZE,lua_tostring(Lua, -1),strlen(lua_tostring(Lua, -1)));
	lua_pop(Lua, 1);

	MakeErmErrorMessage(message,ERR_BUFSIZE,sp,m,Num, traceback);

	char error_message[ERR_BUFSIZE]="";
	//sprintf_s(error_message,ERR_BUFSIZE,"%s\n\n%s\nSave all ERM vars to WOGVARLOG.TXT (may take time)?",err_mess,message);
	sprintf_s(error_message,ERR_BUFSIZE,"%s\n\n%s\n",err_mess,message);
	/*if(UniversalErrorMessage(error_message,WOGERMLOG,2) == MESSAGE_YES) {	DumpERMVars(err_mess,false); } // Commented because there is no need for dump if it's Lua*/
	UniversalErrorMessage(error_message,WOGERMLOG,1);
}

/****************************************************************************************/
// Old error functions

void _Error(int File,int Line)
{
	STARTNA(__LINE__, 0)
	char err_mess[ERR_BUFSIZE]="";
	if(File==0){
		//if(WoGType){ Zsprintf2(&TermStruct,"Ошибка в скрипте ERM. Причина не указана.",0,0); }
		//else{ Zsprintf2(&TermStruct,"ERM script Error. Reason unknown.",0,0); }
		sprintf_s(err_mess,ERR_BUFSIZE,"ERM script Error. Reason unknown.");
	}else{
		//if(WoGType){ Zsprintf2(&TermStruct,"Ошибка в скрипте ERM. \n\tФайл: %s\n\tСтрока : %i",(Dword)SourceFileList[File],(Dword)Line); }
		//else{ Zsprintf2(&TermStruct,"ERM script Error. \n\tFile: %s\n\tLine : %i",(Dword)SourceFileList[File],(Dword)Line); }
		sprintf_s(err_mess,ERR_BUFSIZE,"ERM script Error. \n\tFile: %s\n\tLine : %i",SourceFileList[File],Line);
	}
	UniversalErrorMessage(err_mess,WOGERMLOG);
	RETURNV
}

bool DoneError;
ErrStringInfo ErrString;
void _MError(int File,int Line,char *Txt)
{
	if(DoneError) return;
	STARTNA(__LINE__, 0)
	PL_ERMError=1;
	DoneError = true;
	if(ErrString.str == LuaErrorString)
	{
		LuaLastError(Txt);
		RETURNV
	}

	if(PL_ERMErrDis) RETURNV
	//if(Txt==0){
	//	_Error(File,Line);
	//}else{
	if(Txt==0){
		Txt = (WoGType ? "неизвестна" : "unknown");
	}
	char *ErrorStr = (WoGType
		? "Ошибка в скрипте ERM.\n\tFile: %s\n\tLine : %i\n\tПричина:\n\t%s\n\nСохранить дамп ERM переменных в LOGS/WOGVARLOG.TXT (длит. процедура)?"
		: "ERM script Error.\n\tFile: %s\n\tLine : %i\n\tReason:\n\t%s\n\nSave all ERM vars to WOGVARLOG.TXT (may take time)?"
	);
	
	char err_mess[ERR_BUFSIZE]="";
	sprintf_s(err_mess,ERR_BUFSIZE,ErrorStr, SourceFileList[File], (Dword)Line, Txt);
	//Zsprintf3(&TermStruct, ErrorStr, (Dword)SourceFileList[File], (Dword)Line, (Dword)Txt);
	//}
	//if(Request0(TermStruct.Str))
	if(UniversalErrorMessage(err_mess,WOGERMLOG,2) == MESSAGE_YES)
	{
		DumpERMVars(err_mess,false); 
	}
	RETURNV
}
void _TError(int File,int Line,char *Txt)
{
	STARTNA(__LINE__, 0)
	char err_mess[ERR_BUFSIZE]="";
	if(Txt==0){
		sprintf_s(err_mess,ERR_BUFSIZE,"Error. \n\tFile: %s\n\tLine : %i",(Dword)SourceFileList[File],(Dword)Line);
	}else{
		if(WoGType){ sprintf_s(err_mess,ERR_BUFSIZE,"Ошибка.\n\tФайл: %s\n\tСтрока: %i\n\tПричина:\n\t%s",(Dword)SourceFileList[File],(Dword)Line,(Dword)Txt); }
		else{ sprintf_s(err_mess,ERR_BUFSIZE,"Error.\n\tFile: %s\n\tLine: %i\n\tReason:\n\t%s",(Dword)SourceFileList[File],(Dword)Line,(Dword)Txt); }
	}
	UniversalErrorMessage(err_mess);
	RETURNV
}

void DumpMessage(char *txt,int offset){
	STARTNA(__LINE__, 0)
	char err_mess[ERR_BUFSIZE]="";
	strncpy(err_mess,&txt[offset],ERR_BUFSIZE);
	UniversalErrorMessage(err_mess);
	RETURNV
}

static char ME_Buf2[1000000]="";
void DumpERMVars(char *Text, bool NoLuaTraceback)
{
	// 3.58
	STARTNA(__LINE__, 0)
	int hout;
	char *p = ME_Buf2;
	char *p2 = p + sizeof(ME_Buf2);
	p += sprintf_s(p, p2-p, "-----------------------\n%s\n-----------------------\n",Text);
	time_t ltime;
	time(&ltime);
	p += sprintf_s(p, p2-p, "Time Stamp: %s\n", asctime(gmtime( &ltime )));
	p += sprintf_s(p, p2-p, "WoG Version: %s\n\n", WOG_STRING_VERSION);
	p += sprintf_s(p, p2-p, "Map Saved with: %s\n\n", MapSavedWoG);

	/*// !!! There's segfault
	if(ErrString.str){
		p += sprintf_s(p, p2-p, "ERM stack traceback:\n");
		// this loop itself causes segfault. I've commented both commands inside and nothing changed
		for(ErrStringInfo *e = &ErrString; (e) && (e->str); e = e->last){
			p += sprintf_s(p, p2-p, "\n%s", LuaPushERMInfo(e->str));
			lua_pop(Lua, 1);
		}
		p += sprintf_s(p, p2-p, "\n-----------------\n");
	}
	if(!NoLuaTraceback){
		LuaCallStart("traceback");
		LuaPCall(0, 1);
		p += sprintf_s(p, p2-p, "Lua %s\n-----------------\n", lua_tostring(Lua, -1));
		lua_pop(Lua, 1);
	}*/

	p += sprintf_s(p, p2-p, "COMMON VARS\n");
	p += sprintf_s(p, p2-p, "f=%i\ng=%i\nh=%i\ni=%i\nj=%i\nk=%i\nl=%i\nm=%i\nn=%i\no=%i\np=%i\nq=%i\nr=%i\ns=%i\nt=%i\n",
		ERMVar[0],ERMVar[1],ERMVar[2],ERMVar[3],ERMVar[4],
		ERMVar[5],ERMVar[6],ERMVar[7],ERMVar[8],ERMVar[9],
		ERMVar[10],ERMVar[11],ERMVar[12],ERMVar[13],ERMVar[14]);
	int i,j;
	p += sprintf_s(p, p2-p, "Common flags (1...1000)\n");
	for(i=0;i<1000;i++){ 
		if(ERMFlags[i]==0) continue;
		p += sprintf_s(p, p2-p, "flag%i=%i\n",i+1,ERMFlags[i]); 
	}
	p += sprintf_s(p, p2-p, "Common v vars (v1...v10000)\n");
	for(i=0;i<VAR_COUNT_V;i++){ 
		if(ERMVar2[i]==0) continue;
		p += sprintf_s(p, p2-p, "v%i=%i\n",i+1,ERMVar2[i]); 
	}
	p += sprintf_s(p, p2-p, "Hero's vars (w1...w200)\n");
	for(i=0;i<HERNUM;i++){
		hout=0;
		for(j=0;j<200;j++){ 
			if(ERMVarH[i][j]==0) continue;
			if(hout==0){ p += sprintf_s(p, p2-p, "Hero#=%i\n",i); hout=1; }
			p += sprintf_s(p, p2-p, "w%i=%i\n",j+1,ERMVarH[i][j]); 
		}
	}
	p += sprintf_s(p, p2-p, "\nTRIGGER BASED VARS\n");
	p += sprintf_s(p, p2-p, "Trigger y vars (y-1...y-100)\n");
	for(i=0;i<100;i++){ 
		if(ERMVarYT[i]==0) continue;
		p += sprintf_s(p, p2-p, "y-%i=\"%i\"\n",i+1,ERMVarYT[i]); 
	}
	p += sprintf_s(p, p2-p, "Trigger e vars (e-1...e-100)\n");
	for(i=0;i<100;i++){ 
		if(ERMVarFT[i]==0.0) continue;
		p += sprintf_s(p, p2-p, "e-%i=\"%f\"\n",i+1,ERMVarFT[i]); 
	}

	p += sprintf_s(p, p2-p, "\nFUNCTION BASED VARS\n");
	p += sprintf_s(p, p2-p, "Parameters x vars (x1...x16)\n");
	for(i=0;i<16;i++){ 
		if(ERMVarX[i]==0) continue;
		p += sprintf_s(p, p2-p, "x%i=\"%i\"\n",i+1,ERMVarX[i]); 
	}
	p += sprintf_s(p, p2-p, "Local y vars (y1...y100)\n");
	for(i=0;i<100;i++){ 
		if(ERMVarY[i]==0) continue;
		p += sprintf_s(p, p2-p, "y%i=\"%i\"\n",i+1,ERMVarY[i]); 
	}
	p += sprintf_s(p, p2-p, "Local e vars (e1...e100)\n");
	for(i=0;i<100;i++){ 
		if(ERMVarF[i]==0.0) continue;
		p += sprintf_s(p, p2-p, "e%i=\"%f\"\n",i+1,ERMVarF[i]); 
	}
	p += sprintf_s(p, p2-p, "\nSTRING VARS\n");
	p += sprintf_s(p, p2-p, "Common z vars (z1...z1000)\n");
	for(i=0;i<1000;i++){ 
		if(ERMString[i][0]==0) continue;
		p += sprintf_s(p, p2-p, "z%i=\"%s\"\n",i+1,ERMString[i]); 
	}
	p += sprintf_s(p, p2-p, "Local z vars (z-1...z-20)\n");
	for(i=0;i<VAR_COUNT_LZ;i++){ 
		if(ERMLString[i][0]==0) continue;
		p += sprintf_s(p, p2-p, "z-%i=\"%s\"\n",i+1,ERMLString[i]); 
	}
	p += sprintf_s(p, p2-p, "\n-----------------\n");

	SaveSetupState(WOGVARLOG,ME_Buf2,strlen(ME_Buf2));
	RETURNV
}

