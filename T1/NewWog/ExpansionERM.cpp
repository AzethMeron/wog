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
int FindCurseBlockObject(int index, int type, int subtype) // -1 means not found
{
	for(int i = 0; i < CURSE_BLOCKS; ++i)
	{
		if(DHVC_Table[i][0] != index) continue;
		if(DHVC_Table[i][1] != type) continue;
		if(DHVC_Table[i][2] != subtype) continue;
		return i;
	}
	return -1;
}
int FindEndOfBlockedObjects() // find first {0,0,0}
{
	for(int i = 0; i < CURSE_BLOCKS; ++i)
	{
		if(DHVC_Table[i][0] != 0) continue;
		return i;
	}
	return -1;
}
int AddCurseBlockObject(int index, int type, int subtype)
{
	int entry = FindCurseBlockObject(index,type,subtype);
	if(entry != -1) return 0;
	entry = FindEndOfBlockedObjects();
	if(entry == -1) return 1;
	DHVC_Table[entry][0] = index;
	DHVC_Table[entry][1] = type;
	DHVC_Table[entry][2] = subtype;
	if(entry < CURSE_BLOCKS-1) 
	{
		DHVC_Table[entry+1][0] = 0;
		DHVC_Table[entry+1][1] = 0;
		DHVC_Table[entry+1][2] = 0;
	}
	return 0;
}
void RemoveCurseBlockObject(int index, int type, int subtype)
{
	int entry = FindCurseBlockObject(index,type,subtype);
	if(entry == -1) return;
	int end = FindEndOfBlockedObjects(); 
	if(end == -1) end = CURSE_BLOCKS;
	for(int i = 0; i < 3; ++i)
	{
		DHVC_Table[entry][i] = DHVC_Table[end-1][i];
		DHVC_Table[end-1][i] = 0;
	}
}
int ERM_CurseSetup(char Cmd,int Num,_ToDo_* sp,Mes *Mp)
{
	STARTNA(__LINE__, 0)
	int index = GetVarVal(&sp->Par[0]);
	if(index < 1 || index >= CURSETYPE_NUM){ MError2("incorrect curse/blessing index."); RETURN(0) }
	switch(Cmd)
	{
		case 'P': // Get/set picture
		{
			CHECK_ParamsNum(1);
			StrMan::Apply(CurseType[index].PicName,Mp,0,sizeof(CurseType[index].PicName));
		} break;

		case 'D': // Get/set description
		{
			CHECK_ParamsNum(1);
			StrMan::Apply(CurseType[index].Desc,Mp,0, sizeof(CurseType[index].Desc));
		} break;

		case 'B': // Forbid entering objects
		{
			CHECK_ParamsNum(3);
			int type=0, subtype=0;
			if(Apply(&type,sizeof(type),Mp,0)) { MError2("Cannot get parameter: Type"); RETURN(0); }
			if(Apply(&subtype,sizeof(type),Mp,1)) { MError2("Cannot get parameter: Subtype"); RETURN(0); }
			if(type==63 && subtype != 0) { MError2("Cannot forbid entrance to WoG objects"); RETURN(0); } 
			int disabled = 0; if(FindCurseBlockObject(index,type,subtype)!=-1) disabled = 1;
			if(!Apply(&disabled,sizeof(disabled),Mp,2))
			{
				if(disabled == 1)
				{
					if(AddCurseBlockObject(index,type,subtype)) { MError2("Too many entries in objects-blocked-by-curse array"); RETURN(0);}
				} else if(disabled == 0)
				{
					RemoveCurseBlockObject(index,type,subtype);
				} else { MError2("unknown operation:"); RETURN(0);}
			}
		} break;

		case 'X': // Log Curses/Blesses
		{
			CHECK_ParamsNum(2);
			char filename[64];
			if(StrMan::Apply(filename,Mp,0,256) == 0) { MError2("Cannot get parameter: logfile name"); RETURN(0);}
			int skip_empty_entries = 1;
			if(Apply(&skip_empty_entries,sizeof(skip_empty_entries),Mp,1)) { MError2("Cannot get parameter: skip empty entries"); RETURN(0); }
			FILE* file = fopen(filename,"at");
			if(file == NULL) { MError2("Cannot open logfile"); RETURN(0);}
			else
			{
				fprintf(file, "Logging all initialised Curses/Blesses\n");
				for(int i = 1; i < CURSETYPE_NUM; ++i)
				{
					if(skip_empty_entries) if(!strcmp(CurseType[i].PicName,"")) continue;
					fprintf(file,"Type %d, PictureName '%s', Description '%s'\n",i,CurseType[i].PicName,CurseType[i].Desc);
				}
				fprintf(file, "Logging all entries in Objects blocked by Curses\n");
				for(int i = 0; i < CURSE_BLOCKS; ++i)
				{
					if(skip_empty_entries) if(DHVC_Table[i][0]==0) continue;
					fprintf(file,"Curse index %d, Object type %d, Object subtype %d\n",DHVC_Table[i][0],DHVC_Table[i][1],DHVC_Table[i][2]);
				}
				fclose(file);
			}
		} break;

		default:
			{ EWrongCommand(); RETURN(0); }
			break;
	}
	RETURN(1);
}
/****************************************************************************************/