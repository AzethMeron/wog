#include "../common.h"
#include "../structs.h"
#include "../b1.h"
#include "../erm.h"
#include "../herospec.h"
#include "../string.h"
#include "ExpansionERM.h"
#include <cmath>
#include <cstring>
#include "../service.h"

#define __FILENUM__ 33
#include "List.h"

// by Jakub Grzana

/************************************ NEW VARIABLES *************************************/
// _DataBlock_ replaced new variables structure. It is more universal, tho harder to use by scripter
struct _DataBlock_ {
	// storage
	int length;
	Byte* data;
	// constructor, destructor
	_DataBlock_(int len) {
		if(len <= 0) 
		{ 
			MError("(internal) trying to allocate less than 0 bytes"); length = 0; data = NULL; // should be managed in layer above, in ERM func 
		}
		else 
		{ 
			length = len;
			data = new Byte[len];
			for(int i = 0; i < len; ++i) 
			{ data[i] = '\0'; } // fill with zeros
		}
	}
	~_DataBlock_() { 
		delete data; 
	}
	// remove copy-able
	private: _DataBlock_(const _DataBlock_&);
	private: _DataBlock_();
};

_List_<_DataBlock_> datablocks_stored;
_List_<_DataBlock_> datablocks_temp;
_List_<_DataBlock_> datablocks_perm;

void AddDataBlock(_List_<_DataBlock_>* list, const char name[], int len)
{
	STARTNA(__LINE__, 0)
	_DataBlock_* block = list->find(name);
	if(block) // exists
	{
		MError("Trying to create multiple multiple datablocks with the same name"); // should be managed in layer above, by ERM function
	}
	else // null
	{
		block = new _DataBlock_(len);
		list->push_front(name,block);
	}
	RETURNV;
}

void RemoveDataBlock(_List_<_DataBlock_>* list, const char name[])
{
	STARTNA(__LINE__, 0)
	_DataBlock_* block = list->find(name);
	if(block) // exists
	{
		list->remove(name);
		delete block;
	}
	else // doesnt exist
	{
		// should be managed by layer above, in ERM function
		MError("(internal) Trying to remove non-existant block");
	}
	RETURNV
}

int save_single_datablock_list(_List_<_DataBlock_>* list)
{
	STARTNA(__LINE__, 0)
	unsigned int num = list->size();
	if(Saver(&num,sizeof(num))) { RETURN(1); }
	_ListIterator_<_DataBlock_> iterator = list->begin();
	for(unsigned int i = 0; i < num; ++i)
	{
		if(Saver( iterator.id(), ID_LENGTH)) { RETURN(1); }
		_DataBlock_* current = *iterator;
		if(Saver( &current->length, sizeof(current->length))) { RETURN(1); }
		if(Saver( current->data, current->length ) ) { RETURN(1); }
		++iterator;
	}
	RETURN(0)
}

int load_single_datablock_list(_List_<_DataBlock_>* list)
{
	STARTNA(__LINE__, 0)
	unsigned int num = 0;
	if(Loader(&num,sizeof(num)) ) { RETURN(1); }
	for(unsigned int i = 0; i < num; ++i)
	{
		char name[ID_LENGTH] = "";
		if(Loader(name,sizeof(name)) ) { RETURN(1); }
		int length = 0;
		if(Loader(&length,sizeof(length))) { RETURN(1); }
		AddDataBlock(list, name, length);
		if(Loader(list->front()->data,length*sizeof(Byte))) { RETURN(1); }
	}
	RETURN(0)
}

void reset_single_datablock_list(_List_<_DataBlock_>* list)
{
	STARTNA(__LINE__, 0)
	unsigned int num = list->size();
	for(unsigned int i = 0; i < num; ++i)
	{
		_DataBlock_* current = list->front();
		list->pop_front();
		delete current;
	}
	RETURNV
}

int save_datablocks()
{
	STARTNA(__LINE__, 0)
	if(Saver("varl",4)) { RETURN(1); }
	// data
	save_single_datablock_list(&datablocks_stored);
	if(Saver("lrav",4)) { RETURN(1); }
	RETURN(0)
}

int load_datablocks()
{
	STARTNA(__LINE__, 0)
	// header
	char head_buf[4] = "";
	if(Loader(&head_buf,4)) RETURN(1);
	if(head_buf[0] != 'v' || head_buf[1] != 'a' || head_buf[2] != 'r' || head_buf[3] != 'l') { MError("Malformed savefile - failed to load Datablocks header"); RETURN(1); }
	
	// data
	load_single_datablock_list(&datablocks_stored);

	// End Mark
	if(Loader(&head_buf,4)) RETURN(1);
	if(head_buf[0] != 'l' || head_buf[1] != 'r' || head_buf[2] != 'a' || head_buf[3] != 'v') { MError("Malformed savefile - failed to load Datablocks end mark"); RETURN(1); }
	RETURN(0)
}

int reset_datablocks()
{
	STARTNA(__LINE__, 0)
	reset_single_datablock_list(&datablocks_stored);
	reset_single_datablock_list(&datablocks_temp);
	RETURN(0)
}
	

int ERM_VarList(char Cmd,int Num,_ToDo_* sp,Mes *Mp)
{
	STARTNA(__LINE__, 0);

	int mode = GetVarVal(&sp->Par[0]);
	_List_<_DataBlock_>* list = NULL;

	if(mode == 0) { list = &datablocks_stored;}
	else if(mode == 1) { list = &datablocks_temp; }
	else if(mode == 2) { list = &datablocks_perm; }
	else { MError2("Unknown 'mode' of DataBlock"); RETURN(0); }

	switch(Cmd)
	{
		// LOW-LEVEL
		case 'C': { // check whether datablock already exists
			CHECK_ParamsMin(2);
			CHECK_ParamsMax(3);
			char name[ID_LENGTH] = "";
			if(StrMan::Apply(name,Mp,0,ID_LENGTH) == 0 ) { MError2("Cannot get name of DataBlock"); RETURN(0); }
			_DataBlock_* ptr = list->find(name);
			int address = NULL;
			int length = -1;
			if(ptr) {
				address = (int)ptr->data;
				length = ptr->length;
			}
			Apply(&address,sizeof(address),Mp,1);
			if(Num==3) {
				Apply(&length,sizeof(length),Mp,2);
			}
		} break;

		case 'R': { // remove block
			CHECK_ParamsNum(1);
			char name[ID_LENGTH] = "";
			if(StrMan::Apply(name,Mp,0,ID_LENGTH) == 0 ) { MError2("Cannot get name of DataBlock"); RETURN(0); }
			_DataBlock_* ptr = list->find(name);
			if(ptr)
			{
				RemoveDataBlock(list,name);
			}
			else
			{
				MError2("Attempting to remove non-existant DataBlock"); RETURN(0);
			}
		} break;

		/*case 'A': { // direct access to data, obsolete at this point
			CHECK_ParamsNum(2);
			char name[ID_LENGTH] = "";
			if(StrMan::Apply(name,Mp,0,ID_LENGTH) == 0 ) { MError2("Cannot get name of DataBlock"); RETURN(0); }
			_DataBlock_* block = list->find(name);
			if(block == NULL) { MError2("Accessing non-existant DataBlock"); RETURN(0); }
			if(Mp->VarI[1].Type == 7) // z-strings
			{
				StrMan::Apply((char*)block->data,Mp,1,block->length);
			}
			else // integers 
			{
				Apply(block->data,4,Mp,1);
			}
		} break;*/

		// HIGH-LEVEL
		case 'B': { // array of bytes - interface for easier accessing
			CHECK_ParamsMin(2);
			CHECK_ParamsMax(3);
			char name[ID_LENGTH] = "";
			if(StrMan::Apply(name,Mp,0,ID_LENGTH) == 0 ) { MError2("Cannot get name of DataBlock"); RETURN(0); }
			if(Num==2) // creating
			{
				int length = 0;
				if(Apply(&length,sizeof(length),Mp,1)) { MError2("Cannot get length of DataBlock"); RETURN(0); }
				if(list->find(name)) { MError2("DataBlock with given name already exists"); RETURN(0); }
				AddDataBlock(list,name,length);
			} 
			else // accessing
			{
				int index = 0;
				if(Apply(&index,sizeof(index),Mp,1)) { MError2("Cannot get index"); RETURN(0); }
				_DataBlock_* block = list->find(name);
				if(!block) { MError2("DataBlock with given name doesn't exist"); RETURN(0); }
				// optimalisation trick: let's move _DataBlock_ to the beginning, so next access will be O(1) instead of O(n)
				if(block != list->front()) {
					if(!list->remove(name)) { MError2("Internal error (can't move datablock to front"); RETURN(0); }
					list->push_front(name, block);
				}
				// end of optimalisation trick
				if(index > block->length - sizeof(Byte) || index < 0) { MError2("Invalid index (out of boundaries)"); RETURN(0); }
				Apply(&block->data[index],sizeof(Byte),Mp,2);
			}
		} break;
		case 'V': { // array of v-vars
			CHECK_ParamsMin(2);
			CHECK_ParamsMax(3);
			char name[ID_LENGTH] = "";
			if(StrMan::Apply(name,Mp,0,ID_LENGTH) == 0 ) { MError2("Cannot get name of DataBlock"); RETURN(0); }
			if(Num==2) // creating
			{
				int length = 0;
				if(Apply(&length,sizeof(length),Mp,1)) { MError2("Cannot get length of DataBlock"); RETURN(0); }
				if(list->find(name)) { MError2("DataBlock with given name already exists"); RETURN(0); }
				length = length * sizeof(int);
				AddDataBlock(list,name,length);
			} 
			else // accessing
			{
				int index = 0;
				if(Apply(&index,sizeof(index),Mp,1)) { MError2("Cannot get index"); RETURN(0); }
				_DataBlock_* block = list->find(name);
				if(!block) { MError2("DataBlock with given name doesn't exist"); RETURN(0); }
				// optimalisation trick: let's move _DataBlock_ to the beginning, so next access will be O(1) instead of O(n)
				if(block != list->front()) {
					if(!list->remove(name)) { MError2("Internal error (can't move datablock to front"); RETURN(0); }
					list->push_front(name, block);
				}
				// end of optimalisation trick
				index = index * sizeof(int);
				if(index > block->length - sizeof(int) || index < 0) { MError2("Invalid index (out of boundaries)"); RETURN(0); }
				Apply(&block->data[index],sizeof(Byte),Mp,2);
			}
		} break;

		case 'Z': { // array of z-vars
			CHECK_ParamsMin(2);
			CHECK_ParamsMax(3);
			char name[ID_LENGTH] = "";
			if(StrMan::Apply(name,Mp,0,ID_LENGTH) == 0 ) { MError2("Cannot get name of DataBlock"); RETURN(0); }
			if(Num==2) // creating
			{
				int length = 0;
				if(Apply(&length,sizeof(length),Mp,1)) { MError2("Cannot get length of DataBlock"); RETURN(0); }
				if(list->find(name)) { MError2("DataBlock with given name already exists"); RETURN(0); }
				length = length * 512;
				AddDataBlock(list,name,length);
			} 
			else // accessing
			{
				int index = 0;
				if(Apply(&index,sizeof(index),Mp,1)) { MError2("Cannot get index"); RETURN(0); }
				_DataBlock_* block = list->find(name);
				if(!block) { MError2("DataBlock with given name doesn't exist"); RETURN(0); }
				// optimalisation trick: let's move _DataBlock_ to the beginning, so next access will be O(1) instead of O(n)
				if(block != list->front()) {
					if(!list->remove(name)) { MError2("Internal error (can't move datablock to front"); RETURN(0); }
					list->push_front(name, block);
				}
				// end of optimalisation trick
				if(index > block->length - 512 || index < 0) { MError2("Invalid index (out of boundaries)"); RETURN(0); }
				index = index * 512;
				StrMan::Apply((char*)&block->data[index],Mp,2,512);
			}
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
	switch(Cmd)
	{
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
char SIGNAL_raised[SIGNAL_buffer_size]="";

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
			char buffer[SIGNAL_buffer_size]="";
			if(StrMan::Apply(buffer,Mp,0,SIGNAL_buffer_size) == 0) { MError2("Cannot get signal"); RETURN(0); }
			// backing up previously called signal
			char last_signal[SIGNAL_buffer_size]=""; // i gave you my heart
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
			char buffer[SIGNAL_buffer_size]="";
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

void RemoveSkeletonPatch(int cr_id, bool error_on_absence)
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
	if(head_buf[0] != 'l' || head_buf[1] != 'e' || head_buf[2] != 'k' || head_buf[3] != 's') { MError("Malformed savefile - failed to load Skeleton Patch List end mark"); RETURN(1); }
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

/*********************************** SAVEFILE SUPPORT ***********************************/
// return 0 if everything is fine, otherwise 1
int SaveLegacyData()
{
	STARTNA(__LINE__, 0)
	// Header - for safety purposes
	if(Saver((void*) "JGWL",4)) RETURN(1);
	// Saving data
	if(save_datablocks()) { RETURN(1); }
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
	if(load_datablocks()) { RETURN(1); }
	if(load_SkelPatches()) { RETURN(1); }
	// End Mark
	if(Loader(&head_buf,4)) RETURN(1);
	if(head_buf[0] != 'L' || head_buf[1] != 'W' || head_buf[2] != 'G' || head_buf[3] != 'J') { MError("Malformed savefile - failed to load Legacy data end mark"); RETURN(1); }
	RETURN(0);
}

void ResetLegacyData()
{
	STARTNA(__LINE__, 0)
	reset_datablocks();
	reset_SkelPatches();
	RETURNV;
}
/****************************************************************************************/