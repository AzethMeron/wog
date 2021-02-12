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
#include "List.h"

// by Jakub Grzana

// Structure is send byte after byte. Pointers aren't allowed, though you can use static arrays
// You can add variables here to be automatically saved in savefile. 
// Should work in battles. Assuming that "sending savefile" works just like save/load, then should work in multiplayer aswell. Brief tests suggest that aswell
struct _LegacyGenericData_ {
	char ttt;
} LegacyGenericData;

/************************************ NEW VARIABLES *************************************/
// should be remade to use list implementation from List.h
struct _NewVariable_ {
	_NewVariable_* next;

	char name[32];
	char type;

	char* val_str;
	int val_int;
	// would be better, if both stored in one pointer - Byte* data, then casted to proper representation
	// but waste is 4 bytes per variable, i think it is acceptable XD 
	
	_NewVariable_() { next = NULL; val_str = NULL; val_int = 0; }
};

_NewVariable_* vl_get_next(_NewVariable_* list)
{
	STARTNA(__LINE__, 0)
	if(list == NULL) RETURN(NULL);
	RETURN(list->next)
}

int Apply(_NewVariable_* var, Mes* Mp, char ind)
{
	STARTNA(__LINE__, 0)
	int output = 0;
	if(var->type == 0) { output = Apply(&var->val_int,sizeof(var->val_int),Mp,ind); }
	else if(var->type == 1) { if(StrMan::Apply(var->val_str,Mp,ind,512)) { output = 0; } else { output = 1; } }
	RETURN(output)
}

int var_save(_NewVariable_* var) 
{
	STARTNA(__LINE__, 0)
	if(Saver(var->name,sizeof(var->name))) { RETURN(1); }
	if(Saver(&var->type,sizeof(var->type))) { RETURN(1); }
	if(var->type == 0) { if(Saver(&var->val_int,sizeof(var->val_int))) { RETURN(1); } }
	if(var->type == 1) { if(Saver(var->val_str,512)) { RETURN(1); } }
	RETURN(0)
}

int var_load(_NewVariable_* var)
{
	STARTNA(__LINE__, 0)
	if(Loader(var->name,sizeof(var->name))) { RETURN(1);}
	if(Loader(&var->type,sizeof(var->type))) { RETURN(1);}
	if(var->type == 0) { if(Loader(&var->val_int,sizeof(var->val_int))) { RETURN(1); } }
	if(var->type == 1) { if(var->val_str == NULL) {var->val_str = new char[512];} if(Loader(var->val_str,512)) { RETURN(1); } }
	var->next = NULL;
	RETURN(0)
}

_NewVariable_* vl_save = NULL;
_NewVariable_* vl_temp = NULL;

int custom_string_compare(char* a, char* b, int num)
{
	for(int i = 0; i < num; ++i)
	{
		if(a[i] != b[i]) return 1;
	}
	return 0;
}

_NewVariable_* vl_find(char* name, _NewVariable_* list)
{
	STARTNA(__LINE__, 0)
	_NewVariable_* ptr = list;
	while(ptr)
	{
		if(!custom_string_compare(name,ptr->name,32)) RETURN(ptr)
		ptr = vl_get_next(ptr);
	}
	RETURN(NULL)
}

void vl_clear(_NewVariable_* &list)
{
	STARTNA(__LINE__, 0)
	_NewVariable_* ptr_prev = list;
	_NewVariable_* ptr_next = NULL;
	if(list) ptr_next = list->next;
	while(ptr_prev)
	{
		delete ptr_prev;
		ptr_prev = ptr_next;
		if(ptr_next) ptr_next = ptr_next->next;
	}
	list = NULL;
	RETURNV
}

int vl_add(char* name, char type, _NewVariable_* &list)
{
	STARTNA(__LINE__, 0)
	if(vl_find(name,list)) { RETURN(1); }
	_NewVariable_* ptr_prev = list;
	_NewVariable_* ptr_next = NULL;
	if(list) ptr_next = vl_get_next(list);
	while(ptr_next)
	{
		ptr_prev = ptr_next;
		ptr_next = vl_get_next(ptr_next);
	}
	if(!list) { list = new _NewVariable_; ptr_next = list; }
	else { ptr_next = new _NewVariable_;  ptr_prev->next = ptr_next; }
	ptr_next->type = type;
	sprintf_s(ptr_next->name,32,"%s",name);
	if(type == 1) ptr_next->val_str = new char[512];
	RETURN(0)
}

void vl_remove(char* name, _NewVariable_* &list)
{
	STARTNA(__LINE__, 0)
	_NewVariable_* ptr_prev = list;
	_NewVariable_* ptr_next = NULL;
	if(list) // exception - if first element
	{
		if(custom_string_compare(name,list->name,32) == 0)
		{
			ptr_next = list->next;
			if(list->val_str) delete list->val_str;
			delete list;
			list = ptr_next;
			RETURNV;
		}
	}
	while(ptr_prev) // every other
	{
		ptr_next = ptr_prev->next;
		if(ptr_next)
		{
			if(custom_string_compare(name,ptr_next->name,32) == 0)
			{
				if(ptr_next->val_str) delete ptr_next->val_str;
				ptr_prev->next = ptr_next->next;
				delete ptr_next;
				RETURNV;
			}
		}
		ptr_prev = ptr_next;
	}
	RETURNV
}

unsigned int vl_calculate(_NewVariable_* list)
{
	STARTNA(__LINE__, 0)
	unsigned int output = 0;
	_NewVariable_* ptr_prev = list;
	_NewVariable_* ptr_next = NULL;
	if(list) ptr_next = list->next;
	while(ptr_prev)
	{
		++output;
		ptr_prev = ptr_next;
		if(ptr_next) ptr_next = ptr_next->next;
	}
	RETURN(output)
}

int save_vl()
{
	STARTNA(__LINE__, 0)
	if(Saver("varl",4)) { RETURN(1); }
	// data
	unsigned int size_save = vl_calculate(vl_save); 
	if(Saver(&size_save,sizeof(size_save))) { RETURN(1); }

	for(_NewVariable_* ptr = vl_save; ptr; ptr = vl_get_next(ptr))
	{
		var_save(ptr);
	}
	
	if(Saver("lrav",4)) { RETURN(1); }
	RETURN(0)
}

int load_vl()
{
	STARTNA(__LINE__, 0)
	// header
	char head_buf[4] = "";
	if(Loader(&head_buf,4)) RETURN(1);
	if(head_buf[0] != 'v' || head_buf[1] != 'a' || head_buf[2] != 'r' || head_buf[3] != 'l') { MError("Malformed savefile - failed to load New Var List header"); RETURN(1); }
	
	// data
	unsigned int size_save = 0;
	if(Loader(&size_save,sizeof(size_save))) { RETURN(1); }

	for(unsigned int i = 0; i < size_save; ++i)
	{
		_NewVariable_* ptr = new _NewVariable_;
		if(var_load(ptr)) { RETURN(1); }
		ptr->next = vl_save;
		vl_save = ptr;
	}

	// End Mark
	if(Loader(&head_buf,4)) RETURN(1);
	if(head_buf[0] != 'l' || head_buf[1] != 'r' || head_buf[2] != 'a' || head_buf[3] != 'v') { MError("Malformed savefile - failed to load New Var List end mark"); RETURN(1); }
	RETURN(0)
}
	

int ERM_VarList(char Cmd,int Num,_ToDo_* sp,Mes *Mp)
{
	STARTNA(__LINE__, 0);

	int mode = GetVarVal(&sp->Par[0]);
	_NewVariable_** list = NULL;

	if(mode == 0) { list = &vl_save;}
	else if(mode == 1) { list = &vl_temp; }
	else { MError2("Unknown 'mode' of variable"); RETURN(0); }

	switch(Cmd)
	{
		case 'I':
		{
			CHECK_ParamsNum(2);
			char name[32] = "";
			if(StrMan::Apply(name,Mp,0,32) == 0) { MError2("Cannot get 'name' of variable"); RETURN(0); }
			int type = 0;
			if(type != 0 && type != 1) { MError2("Unrecognised 'type'"); RETURN(0); }
			if(Apply(&type,sizeof(type),Mp,1)) { MError2("Cannot get 'type' of variable"); RETURN(0); }
			if(vl_find(name,*list)) { MError2("Name already taken"); RETURN(0); }
			vl_add(name,type,*list);
		} break;

		case 'A':
		{
			CHECK_ParamsMin(2);
			char name[32] = "";
			if(StrMan::Apply(name,Mp,0,32) == 0) { MError2("Cannot get 'name' of variable"); RETURN(0); }
			_NewVariable_* var = vl_find(name,*list);
			if(Num > 2) { char type = 0; Apply(&type,sizeof(type),Mp,2); if(type != 0 && type != 1) { MError2("Incorrect type"); RETURN(0); } vl_add(name,type,*list); var = vl_find(name,*list); }
			if(var)
			{
				Apply(var,Mp,1); 
			}
			else
			{
				MError2("Memory slot not found"); RETURN(0); 
			}
		} break;

		case 'U': { Message(vl_calculate(*list));  } break;

		case 'E':
		{
			vl_clear(*list);			
		} break;

		case 'R':
		{
			CHECK_ParamsNum(1);
			char name[32] = "";
			if(StrMan::Apply(name,Mp,0,32) == 0) { MError2("Cannot get 'name' of variable"); RETURN(0); }
			vl_remove(name,*list);
		} break;

		case 'C':
		{
			CHECK_ParamsMin(2);
			char name[32] = "";
			if(StrMan::Apply(name,Mp,0,32) == 0) { MError2("Cannot get 'name' of variable"); RETURN(0); }
			int exist = 0;
			_NewVariable_* ptr = vl_find(name,*list);
			if(ptr) { exist = 1; }
			int type = -1;
			if(ptr) { type = ptr->type; }
			if(Apply(&exist,sizeof(exist),Mp,1) == 0) { MError2("'Exist' parameter is read-only");   RETURN(1); }
			if(Num >= 3) if(Apply(&type,sizeof(type),Mp,2) == 0) { MError2("'Type' parameter is read-only"); RETURN(1); }
		} break;

		default:
			{ EWrongCommand(); RETURN(0); } break;
	}

	RETURN(1);
}

/****************************************************************************************/



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

		case 'S': // Szkieletornia
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
			CHECK_ParamsNum(2);
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
			CHECK_ParamsNum(1);
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

/****************************************************************************************/
// Signals
/*#define SIGNAL_buffer_size (64)
bool SIGNAL_entered_block = false;
char SIGNAL_raised[SIGNAL_buffer_size];

int ERM_Signal(char Cmd,int Num,_ToDo_* sp,Mes *Mp)
// doesn't fully operational yet
// Y-vars are not supported properly yet - keeps value between signal block and block above, which is a bug, not a feature here
{
	STARTNA(__LINE__, 0)

	switch(Cmd)
	{
		case 'R': 
		{
			CHECK_ParamsNum(1);
			// Manage parameter
			char buffer[SIGNAL_buffer_size];
			if(StrMan::Apply(buffer,Mp,0,SIGNAL_buffer_size) == 0) { MError2("Cannot get signal"); RETURN(0); }
			// backing up previously called signal
			char last_signal[SIGNAL_buffer_size]; // i gave you my heart
			memcpy(last_signal,SIGNAL_raised,SIGNAL_buffer_size);
			memcpy(SIGNAL_raised,buffer,SIGNAL_buffer_size);
			// preparing before signal execution
			Mes *lastMp = LastFUMes;
			int lastNum = LastFUNum;
			LastFUMes = Mp;
			LastFUNum = Num;
			SIGNAL_entered_block = true;
			int last_ptr = pointer;
			int yvars = AccessYVarInsideFunction();
			AccessYVarInsideFunction() = 1;
			pointer = 30379;
			// execution
			ProcessERM(true);
			// cleaning after signal execution
			pointer = last_ptr;
			AccessYVarInsideFunction() = yvars;
			SIGNAL_entered_block = false;
			LastFUMes = lastMp;
			LastFUNum = lastNum;
			memcpy(SIGNAL_raised,last_signal,SIGNAL_buffer_size);
		} break;

		case 'G':
		{
			CHECK_ParamsNum(1);
			if(!SIGNAL_entered_block) { MError2("Cannot get raised signal outside of signal trigger"); RETURN(0); }
			char buffer[SIGNAL_buffer_size];
			memcpy(buffer,SIGNAL_raised,SIGNAL_buffer_size);
			if(StrMan::Apply(buffer,Mp,0,SIGNAL_buffer_size)) { MError2("Cannot set signal"); RETURN(0); }
		} break;

		default:
			{ EWrongCommand(); RETURN(0); } break;
	}

	RETURN(1)
}*/

/****************************************************************************************/

struct SkelTransPatch {
	//int cr_id; Since im using generic _List_, i will use 'name' to store creature id. 
	//It makes much sense, im wasting less memory AND problem of overlapping patches is solved
	int cr_new;
	int cr_prev;
};

_List_<SkelTransPatch> ListOfSkelPatches;
// global structure: SkelTrans[cr_id]

// Note: cr_id and cr_new MUST have proper value, check that in ERM section!
void AddSkeletonPatch(int cr_id, int cr_new)
{
	STARTNA(__LINE__, 0)
	// converting cr_id into c-string
	char name_buffer[ID_LENGTH] = "";
	sprintf_s(name_buffer,ID_LENGTH,"%d",cr_id); 
	// looking for existing patch for given creature
	SkelTransPatch* patch = ListOfSkelPatches.find(name_buffer);
	if(patch) // creature with given ID was already changed - not typical situation, but definitely possible
	{
		// Updating existing patch
		SkelTrans[cr_id] = (patch->cr_new = cr_new);
	}
	else
	{
		// Typical situation - creating brand-new patch
		patch = new SkelTransPatch;
		patch->cr_new = cr_new;
		patch->cr_prev = SkelTrans[cr_id];
		SkelTrans[cr_id] = cr_new;
		ListOfSkelPatches.push_front(name_buffer,patch);
	}
	RETURNV;
}

void RemoveSkeletonPatch(int cr_id, bool error_on_absence = false)
{
	STARTNA(__LINE__, 0)
	// converting cr_id into c-string
	char name_buffer[ID_LENGTH] = "";
	sprintf_s(name_buffer,ID_LENGTH,"%d",cr_id); 
	// searching for patch
	SkelTransPatch* patch = ListOfSkelPatches.find(name_buffer);
	if(patch)
	{
		SkelTrans[cr_id] = patch->cr_prev;
		ListOfSkelPatches.remove(name_buffer);
		delete patch;
	}
	else
	{
		if(error_on_absence)
		{
			UniversalErrorMessage("Internal error: trying to revert non-existing Skel Trans Patch");
		}
	}
	RETURNV;
}

int save_SkelPatches()
{
	STARTNA(__LINE__, 0)
	if(Saver("skel",4)) { RETURN(1); }
	// data
	int num = ListOfSkelPatches.size();
	if(Saver(&num,sizeof(num))) { RETURN(1); }
	_ListIterator_<SkelTransPatch> iterator = ListOfSkelPatches.begin();
	for(int i = 0; i < num; ++i)
	{
		if(!iterator.exists()) { UniversalErrorMessage("Internal error while saving Skeleton Patch List. Please report this.\nLoading saved file will be impossible"); RETURN(1); } 
		int cr_id = atoi(iterator.id());
		int cr_new = (*iterator)->cr_new;
		if(Saver(&cr_id,sizeof(cr_id))) { RETURN(1); }
		if(Saver(&cr_new,sizeof(cr_new))) { RETURN(1); }
		++iterator;
	}
	// end of data
	if(Saver("leks",4)) { RETURN(1); }
	RETURN(0);
}

int load_SkelPatches()
{
	STARTNA(__LINE__, 0)
	char head_buf[4] = "";
	if(Loader(&head_buf,4)) RETURN(1);
	if(head_buf[0] != 's' || head_buf[1] != 'k' || head_buf[2] != 'e' || head_buf[3] != 'l') { MError("Malformed savefile - failed to load Skeleton Patch List header"); RETURN(1); }
	// data
	int num = 0;
	if(Loader(&num,sizeof(num))) { RETURN(1); }
	for(int i = 0; i < num; ++i)
	{
		int cr_id = 0;
		int cr_new = 0;
		if(Loader(&cr_id,sizeof(cr_id))) { RETURN(1); }
		if(Loader(&cr_new,sizeof(cr_new))) { RETURN(1); }
		AddSkeletonPatch(cr_id,cr_new);
	}
	// end of data
	if(Loader(&head_buf,4)) RETURN(1);
	if(head_buf[0] != 'l' || head_buf[1] != 'e' || head_buf[2] != 'k' || head_buf[3] != 's') { MError("Malformed savefile - failed to load Skeleton Patch List header"); RETURN(1); }
	RETURN(0);
}

int reset_SkelPatches()
{
	STARTNA(__LINE__, 0)
	int num = ListOfSkelPatches.size();
	for(int i = 0; i < num; ++i)
	{
		int cr_id = atoi(ListOfSkelPatches.id_front());
		RemoveSkeletonPatch(cr_id,true);
	}
	RETURN(0);
}

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
	if(save_vl()) { RETURN(1); }
	if(save_SkelPatches()) { RETURN(1); }
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
	if(load_vl()) { RETURN(1); }
	if(load_SkelPatches()) { RETURN(1); }
	// End Mark
	if(Loader(&head_buf,4)) RETURN(1);
	if(head_buf[0] != 'L' || head_buf[1] != 'W' || head_buf[2] != 'G' || head_buf[3] != 'J') { MError("Malformed savefile - failed to load Legacy data end mark"); RETURN(1); }
	RETURN(0);
}

void ResetLegacyData()
{
	STARTNA(__LINE__, 0)
	vl_clear(vl_save);
	vl_clear(vl_temp);
	reset_SkelPatches();
	RETURNV;
}
/****************************************************************************************/