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

int FindBlessSphinx(short int index)
{
	for(int i = 1; i <= AS_CGood[0][0]; ++i)
	{
		if(index==AS_CGood[i][0]) return i;
	}
	return -1;
}
void RemoveBlessSphinx(short int index)
{
	short int bless_index = FindBlessSphinx(index);
	if(bless_index==-1) return; // exit if there is no such bless
	int end = AS_CGood[0][0];
	if(end == 1) { UniversalErrorMessage("Cannot remove all blessings from sphinx database"); return; }
	for(int i = 0; i < 3; ++i)
	{
		AS_CGood[bless_index][i] = AS_CGood[end][i];
		AS_CGood[end][i] = 0;
	}
	--AS_CGood[0][0]; 
}
int AddBlessSphinx(short int index, short int min, short int max)
{
	short int bless_index = FindBlessSphinx(index);
	if(bless_index==-1)
	{
		bless_index = AS_CGood[0][0] + 1;
		if(bless_index >= CURSETYPE_NUM) { return 1; } 
		++AS_CGood[0][0];
	}
	AS_CGood[bless_index][0] = index;
	AS_CGood[bless_index][1] = min;
	if(max >= min) AS_CGood[bless_index][2] = max;
	else { AS_CGood[bless_index][2] = min; return 1; }
	return 0;
}

int FindCurseSphinx(short int index)
{
	for(int i = 1; i <= AS_CBad[0][0]; ++i)
	{
		if(index==AS_CBad[i][0]) return i;
	}
	return -1;
}
void RemoveCurseSphinx(short int index)
{
	short int curse_index = FindCurseSphinx(index);
	if(curse_index==-1) return;
	int end = AS_CBad[0][0];
	if(end == 1) { UniversalErrorMessage("Cannot remove all curses from sphinx database"); return; }
	for(int i = 0; i < 3; ++i)
	{
		AS_CBad[curse_index][i] = AS_CBad[end][i];
		AS_CBad[end][i] = 0;
	}
	--AS_CBad[0][0];
}
int AddCurseSphinx(short int index, short int min, short int max)
{
	short int curse_index = FindCurseSphinx(index);
	if(curse_index==-1)
	{
		curse_index = AS_CBad[0][0] + 1;
		if(curse_index >= CURSETYPE_NUM) { return 1; } 
		++AS_CBad[0][0];
	}
	AS_CBad[curse_index][0] = index;
	AS_CBad[curse_index][1] = min;
	if(max >= min) AS_CBad[curse_index][2] = max;
	else { AS_CBad[curse_index][2] = min; return 1; }
	return 0;
}

int ERM_CurseSetup(char Cmd,int Num,_ToDo_* sp,Mes *Mp)
{
	STARTNA(__LINE__, 0)
	switch(Cmd)
	{
		case 'P': // Get/set picture
		{
			int index = GetVarVal(&sp->Par[0]);
			if(index < 1 || index >= CURSETYPE_NUM){ MError2("incorrect curse/blessing index(type)."); RETURN(0) }
			CHECK_ParamsNum(1);
			StrMan::Apply(CurseType[index].PicName,Mp,0,sizeof(CurseType[index].PicName));
		} break;

		case 'D': // Get/set description
		{
			int index = GetVarVal(&sp->Par[0]);
			if(index < 1 || index >= CURSETYPE_NUM){ MError2("incorrect curse/blessing index(type)."); RETURN(0) }
			CHECK_ParamsNum(1);
			StrMan::Apply(CurseType[index].Desc,Mp,0, sizeof(CurseType[index].Desc));
		} break;

		case 'B': // Forbid entering objects
		{
			int index = GetVarVal(&sp->Par[0]);
			if(index < 1 || index >= CURSETYPE_NUM){ MError2("incorrect curse/blessing index(type)."); RETURN(0) }
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
				} else if(disabled == 0) {
					RemoveCurseBlockObject(index,type,subtype);
				} else { MError2("unknown operation:"); RETURN(0);}
			}
		} break;

		case 'F': // Find free cursetype
		{
			Message(Random(1,1));
			CHECK_ParamsNum(1);
			int found = -1;
			for(int i = 1; i < CURSETYPE_NUM; ++i)
			{ if(!strcmp(CurseType[i].PicName,"")) if(!strcmp(CurseType[i].Desc,"")) { found = i; break; } }
			if(Apply(&found,sizeof(found),Mp,0) == 0) { MError2("Cannot set output value"); RETURN(0); }
		} break;

		case 'R': // Get random curse/bless
		{
			CHECK_ParamsMin(2);
			int bless = 0;
			Apply(&bless,sizeof(bless),Mp,0);
			int rand_type = -1;
			int rand_val = -1;
			if(bless == 1) // bless
			{
				int maxnum = AS_CGood[0][0];
				int rand_ind = 1; if(maxnum > 1) rand_ind = Random(1,maxnum);
				rand_type = AS_CGood[rand_ind][0];
				if(AS_CGood[rand_ind][1] == AS_CGood[rand_ind][2]) rand_val = AS_CGood[rand_ind][1];
				else rand_val = Random(AS_CGood[rand_ind][1],AS_CGood[rand_ind][2]);
			}
			else if(bless == 0)
			{
				int maxnum = AS_CBad[0][0];
				int rand_ind = 1; if(maxnum > 1) rand_ind = Random(1,maxnum);
				rand_type = AS_CBad[rand_ind][0];
				if(AS_CBad[rand_ind][1] == AS_CBad[rand_ind][2]) rand_val = AS_CBad[rand_ind][1];
				else rand_val = Random(AS_CBad[rand_ind][1],AS_CBad[rand_ind][2]);
			}
			else
			{
				MError2("Invalid parameter value: bless/curse"); RETURN(0);
			}
			if(Apply(&rand_type,sizeof(rand_type),Mp,1) == 0) { MError2("Cannot set output value"); RETURN(0); }
			if(Num==3) if(Apply(&rand_val,sizeof(rand_val),Mp,2) == 0) { MError2("Cannot set output value"); RETURN(0); }
		} break;

		case 'S': // Sphinx support
		{
			int index = GetVarVal(&sp->Par[0]);
			if(index < 1 || index >= CURSETYPE_NUM){ MError2("incorrect curse/blessing index(type)."); RETURN(0) }
			int bless = -1;
			if(Apply(&bless,sizeof(bless),Mp,0)) { MError2("Cannot get parameter: bless or curse"); RETURN(0); }
			switch(bless)
			{
				case 1: // bless
				{
					int command = 0; if(FindBlessSphinx(index)!=-1) command = 1;
					Apply(&command,sizeof(command),Mp,1);
					if(command==0) { RemoveBlessSphinx(index); }
					if(command==1) if(Mp->VarI[1].Check!=1) if(Num!=4) { MError2("Cannot add blessing without specified Min and Max value"); RETURN(0); };
					if(Num!=4 || command==0) break;
					short int min, max;
					if(Apply(&min,sizeof(min),Mp,2)) { MError2("Cannot get parameter: minimum value"); RETURN(0); }
					if(Apply(&max,sizeof(max),Mp,3)) { MError2("Cannot get parameter: maximum value"); RETURN(0); }
					if(max < min) { MError2("Maximum value CANNOT be ower than Minimum"); RETURN(0); }
					if(command==1) { if(AddBlessSphinx(index,min,max)) { MError2("Cannot add more blessings for sphinx"); RETURN(0); } }
				} break;

				case 0: // curse
				{
					int command = 0; if(FindCurseSphinx(index)!=-1) command = 1;
					Apply(&command,sizeof(command),Mp,1);
					if(command==0) { RemoveCurseSphinx(index); }
					if(command==1) if(Mp->VarI[1].Check!=1) if(Num!=4) { MError2("Cannot add curse without specified Min and Max value"); RETURN(0); };
					if(Num!=4 || command==0) break;
					short int min, max;
					if(Apply(&min,sizeof(min),Mp,2)) { MError2("Cannot get parameter: minimum value"); RETURN(0); }
					if(Apply(&max,sizeof(max),Mp,3)) { MError2("Cannot get parameter: maximum value"); RETURN(0); }
					if(max < min) { MError2("Maximum value CANNOT be ower than Minimum"); RETURN(0); }
					if(command==1) { if(AddCurseSphinx(index,min,max)) { MError2("Cannot add more blessings for sphinx"); RETURN(0); } }
				} break;
			
				default:
				{
					MError2("Invalid parameter: bless or curse"); RETURN(0); 
				} break;
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
				fprintf(file, "Logging all entries in Sphinx Curses\n");
				for(int i = 1; i < CURSETYPE_NUM; ++i)
				{
					if(skip_empty_entries) if(AS_CBad[i][0]==0) continue;
					fprintf(file,"Curse index %d, Min %d, Max %d\n",AS_CBad[i][0],AS_CBad[i][1],AS_CBad[i][2]);
				}
				fprintf(file, "Logging all entries in Sphinx Blessings\n");
				for(int i = 1; i < CURSETYPE_NUM; ++i)
				{
					if(skip_empty_entries) if(AS_CGood[i][0]==0) continue;
					fprintf(file,"Bless index %d, Min %d, Max %d\n",AS_CGood[i][0],AS_CGood[i][1],AS_CGood[i][2]);
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