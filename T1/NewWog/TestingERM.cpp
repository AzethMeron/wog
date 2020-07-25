
#include "TestingERM.h"
#include "../common.h"
#include "../structs.h"
#include "../erm.h"
#include <cmath>

#define __FILENUM__ 30

// Header file for Testing ERM Receiver and Trigger
// ~!!!

// Argument type: 2 - x/y/l
int ERM_Testing(char Cmd,int Num,_ToDo_* sp,Mes *Mp)
{
	STARTNA(__LINE__, 0)
	//Dword MixPos=GetDinMixPos(sp);
	//_MapItem_ *mip=GetMapItem2(MixPos);
	switch(Cmd)
	{
		case 'P':
		{
			pointer=30378;
			ProcessERM();
		} break;

		case 'N':
		{
			CHECK_ParamsNum(3)
			int value = 0;
			int modifier = 0;
			if(Apply(&value,4,Mp,0)) { MError2("Can't get value to be rooted"); RETURN(0); }
			if(Apply(&modifier,4,Mp,1)) { MError2("Can't get modifier"); RETURN(0); }
			int output = int( \
				sqrt(double(value)) * double(modifier) \
				);
			if(Apply(&output,4,Mp,2) == 0) { MError2("Can't set output of sqrt"); RETURN(0)}
		} break;

		default:
			EWrongCommand();
			break;
	}
	RETURN(1);
}