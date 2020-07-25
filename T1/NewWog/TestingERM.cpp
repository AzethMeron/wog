
#include "TestingERM.h"
#include "../common.h"
#include "../structs.h"
#include "../erm.h"

// TODO __FILENUM__

// Header file for Testing ERM Receiver and Trigger
// ~!!!

// Argument type: 2 - x/y/l
int ERM_Testing(char Cmd,int Num,_ToDo_* sp,Mes *Mp)
{
	//Dword MixPos=GetDinMixPos(sp);
	//_MapItem_ *mip=GetMapItem2(MixPos);
	switch(Cmd)
	{
		case 'P':
		{
			pointer=30378;
			ProcessERM();
		} break;

		default:
			
			break;
	}
	return 1;
}