#include "../common.h"
#include "../structs.h"
#include "../b1.h"
#include "../erm.h"
#include "../herospec.h"
#include "../string.h"
#include "ExpansionERM.h"
#include <cmath>

#define __FILENUM__ 33

_LegacyGenericData_ LegacyGenericData; 

/****************************** MULTIPLAYER BATTLE SUPPORT ******************************/
#define LegacyDataBufferSize 10000
Byte LegacyDataBuffer[LegacyDataBufferSize]; // Used when sending, but not when receiving

int WriteToBuffer(Byte* buffer, int bufsize, unsigned int index, Byte* data, int datasize)
{
	for(int i = index; i < index+datasize; ++i)
	{
		if(i >= bufsize) return 1;
		buffer[i] = data[i-index];
	}
	return 0;
}

int LoadFromBuffer(Byte* buffer, int bufsize, unsigned int index, Byte* data, int datasize)
{
	for(int i = index; i < index+datasize; ++i)
	{
		if(i >= bufsize) return 1;
		data[i-index] = buffer[i];
	}
	return 0;
}

void SendLegacyData(int* len, Byte** buf)
{
	STARTNA(__LINE__, 0)
	// init
	*len = 0;
	*buf = LegacyDataBuffer;
	// Sending header
	if(WriteToBuffer(LegacyDataBuffer,LegacyDataBufferSize,*len,(Byte*) "JGWL",4) ) MError("Sending legacy data caused overflow");
	*len += 4; // Accquire length of data - in bytes
	// Sending generic data
	if(WriteToBuffer(LegacyDataBuffer,LegacyDataBufferSize,*len,(Byte*) &LegacyGenericData,sizeof(LegacyGenericData)) ) UniversalErrorMessage("Sending legacy data caused overflow");
	*len += sizeof(LegacyGenericData); // Accquire length of data - in bytes
	RETURNV;
}

void ReceiveLegacyData(int len, Byte* buf)
{
	STARTNA(__LINE__, 0)
	// Index in buffer
	int index = 0; 
	// Receiving header
	char head_buf[4] = "";
	if(LoadFromBuffer(buf,len,index,(Byte*) head_buf,4)) MError("Received malformed legacy data");
	if(head_buf[0] != 'J' || head_buf[1] != 'G' || head_buf[2] != 'W' || head_buf[3] != 'L') MError("Received malformed legacy data");
	index += 4;
	// Receiving generic data
	if(LoadFromBuffer(buf,len,index,(Byte*) &LegacyGenericData,sizeof(LegacyGenericData))) MError("Received malformed legacy data");
	index += sizeof(LegacyGenericData);
	RETURNV;
}
/****************************************************************************************/

/*********************************** SAVEFILE SUPPORT ***********************************/
// return 0 if everything is fine, otherwise 1
int SaveLegacyData()
{
	STARTNA(__LINE__, 0)
	// Header - for safety purposes
	if(Saver((void*) "JGWL",4)) RETURN(1);
	// Saving data
	if(Saver(&LegacyGenericData,sizeof(LegacyGenericData))) RETURN(1);
	RETURN(0);
}

// return 0 if everything is fine, otherwise 1
int LoadLegacyData()
{
	STARTNA(__LINE__, 0)
	// Header - for safety purposes
	char head_buf[4] = "";
	if(Loader(&head_buf,4)) RETURN(1);
	if(head_buf[0] != 'J' || head_buf[1] != 'G' || head_buf[2] != 'W' || head_buf[3] != 'L') { MError("Malformed savefile - failed to load Legacy data header"); RETURN(1); }
	// Loading data
	if(Loader(&LegacyGenericData,sizeof(LegacyGenericData))) RETURN(1);
	RETURN(0);
}
/****************************************************************************************/

/************************************ CURSE SUPPORT ************************************/
int ERM_CurseSetup(char Cmd,int Num,_ToDo_* sp,Mes *Mp)
{
	STARTNA(__LINE__, 0)
	int index = GetVarVal(&sp->Par[0]);
	if(index < 1 || index >= CURSETYPE_NUM){ MError2("incorrect curse/blessing index."); RETURN(0) }
	switch(Cmd)
	{
		case 'P': // Get/set picture
		{
			StrMan::Apply(CurseType[index].PicName,Mp,0,sizeof(CurseType[index].PicName));
		} break;
		case 'D': // Get/set description
		{
			StrMan::Apply(CurseType[index].Desc,Mp,0, sizeof(CurseType[index].Desc));
		} break;
		default:
			{ EWrongCommand(); RETURN(0); }
			break;
	}
	RETURN(1);
}
/****************************************************************************************/