#include "../common.h"
#include "../structs.h"
#include "../b1.h"
#include "../erm.h"
#include "../herospec.h"
#include "../string.h"
#include "ExpansionERM.h"
#include <cmath>
#include <cstring>

#define __FILENUM__ 33

// Structure is send byte after byte. Pointers aren't allowed, though you can use static arrays
// You can add variables here to be automatically saved in savefile. 
// Should work in battles. Assuming that "sending savefile" works just like save/load, then should work in multiplayer aswell. Brief tests suggest that aswell
struct _LegacyGenericData_ {
	char ttt;
} LegacyGenericData;

int SkelTransBackup[MONNUM];

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
	if(Saver(&LegacyGenericData,sizeof(LegacyGenericData))) { RETURN(1); }
	if(Saver(&SkelTrans,sizeof(SkelTrans))) { RETURN(1); }
	// End mark
	if(Saver((void*) "LWGJ",4)) RETURN(1);
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
	if(Loader(&LegacyGenericData,sizeof(LegacyGenericData))) { RETURN(1); }
	if(Loader(&SkelTrans,sizeof(SkelTrans))) { RETURN(1); }
	// End Mark
	if(Loader(&head_buf,4)) RETURN(1);
	if(head_buf[0] != 'L' || head_buf[1] != 'W' || head_buf[2] != 'G' || head_buf[3] != 'J') { MError("Malformed savefile - failed to load Legacy data header"); RETURN(1); }
	RETURN(0);
}

void ResetLegacyData()
{
	STARTNA(__LINE__, 0)
	memcpy(&SkelTrans[0],&SkelTransBackup[0],sizeof(SkelTrans));
	RETURNV;
}
/****************************************************************************************/

double round( double fValue )
{
    return fValue < 0 ? ceil( fValue - 0.5 )
        : floor( fValue + 0.5 );
}

// Argument type: 0 - none. It changes frequently
int ERM_Testing(char Cmd,int Num,_ToDo_* sp,Mes *Mp)
{
	STARTNA(__LINE__, 0)
	//Dword MixPos=GetDinMixPos(sp);
	//_MapItem_ *mip=GetMapItem2(MixPos);
	switch(Cmd)
	{
		// Show pointer value
		case 'P':
		{
			Message(Format("(Internal) Pointer value: %lu", pointer));
		} break;

		// Get square root of value. No support for float variables
		case 'R':
		{
			CHECK_ParamsNum(3)
			int value = 0;
			if(Apply(&value,4,Mp,0)) { MError2("Cannot get parameter 1 - value to be rooted"); RETURN(0); }
			int modifier = 0;
			if(Apply(&modifier,4,Mp,1)) { MError2("Cannot get parameter 2 - modifier"); RETURN(0); }
			int output = int( \
				sqrt(double(value)) * double(modifier) \
				);
			if(Apply(&output,4,Mp,2) == 0) { MError2("Cannot set parameter 3 - output value"); RETURN(0)}
		} break;

		// Call any trigger - note it is bad idea, cuz it doesn't make ANY other changes
		case 'C':
		{
			CHECK_ParamsNum(1);
			unsigned long ptr = 0;
			if(Apply(&ptr,sizeof(ptr),Mp,0)) { MError2("Cannot get parameter 1 - pointer"); RETURN(0); }
			pointer=ptr;
			ProcessERM();
		} break;

		case 'S':
		{
			int cr_type = 0;
			if(Apply(&cr_type,sizeof(cr_type),Mp,0)) { MError2("Cannot get creature type"); RETURN(0); }
			if(cr_type < 0 || cr_type >= MONNUM) { MError2("Incorrect creature (source) type"); RETURN(0); }
			int last_cr = SkelTrans[cr_type];
			Apply(&SkelTrans[cr_type],sizeof(SkelTrans[cr_type]),Mp,1);
			if(SkelTrans[cr_type] < 0 || SkelTrans[cr_type] >= MONNUM) { SkelTrans[cr_type] = last_cr; MError2("Incorrect creature (destination) type"); RETURN(0); } 
		} break;

		// Calculate logaritm (n-based) of value
		case 'L':
		{
			CHECK_ParamsNum(4)
			int base = 0;
			if(Apply(&base,4,Mp,0)) { MError2("Cannot get parameter 1 - logbase"); RETURN(0); }
			int value = 0;
			if(Apply(&value,4,Mp,1)) { MError2("Cannot get parameter 2 - value to be logarithmed"); RETURN(0); }
			int modifier = 0;
			if(Apply(&modifier,4,Mp,2)) { MError2("Cannot get parameter 3 - modifier"); RETURN(0); }
			double temp1 = log( double(value) );
			double temp2 = log( double(base) );
			temp1 = temp1 / temp2;
			int output = round(temp1 * double(modifier));
			if(Apply(&output,4,Mp,3) == 0) { MError2("Cannot set parameter 4 - output value"); RETURN(0)}
		} break;

		default:
			{ EWrongCommand(); RETURN(0); }
			break;
	}
	RETURN(1);
}

int ERM_BlackMarket(char Cmd,int Num,_ToDo_* sp,Mes *Mp)
{
	STARTNA(__LINE__, 0)
	Dword MixPos=GetDinMixPos(sp);
	_MapItem_ *mip=GetMapItem2(MixPos);
	if(mip->OType != 7) { MError2("Object isn't Black Market"); RETURN(0); }
	switch(Cmd)
	{
		case 'A':
		{
			_BlackMarketInfo_* market = GetBlackMarket(mip->SetUp);
			int slot;
			if(Apply(&slot,4,Mp,0)) { MError2("Cannot get parameter 1 - slot"); RETURN(0); }
			if(slot < 0 || slot > 6) { MError2("Incorrect parameter 1 value - slot"); RETURN(0); }
			Dword last_art = market->art[slot];
			Apply(&market->art[slot],sizeof(market->art[slot]),Mp,1);
			if(market->art[slot] < 0 || market->art[slot] == 1 || market->art[slot] >= ARTNUM) { MError2("Incorrect artifact number"); market->art[slot] = last_art; RETURN(0); }
		} break;
		case 'I':
		{
			int number_of_black_markets = CalcObjects(7,-1);
			Dword last_SetUp = mip->SetUp;
			Apply(&mip->SetUp,sizeof(mip->SetUp),Mp,0);
			if(mip->SetUp < 0 || mip->SetUp >= number_of_black_markets) { MError2("Wrong index of Black Market"); mip->SetUp=last_SetUp; RETURN(0); }
		} break;
		default:
			{ EWrongCommand(); RETURN(0); } break;
	}
	RETURN(1);
}