#include "../common.h"
#include "../structs.h"
#include "../b1.h"
#include "../erm.h"
#include "../herospec.h"
#include "../string.h"
#include "ExpansionERM.h"
#include <cmath>
#include <cstring>

#define __FILENUM__ 34
#include "List.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

struct allocated_memory {
	Byte* memory; // address in memory
	unsigned int length; // length in bytes

	allocated_memory() {
		memory = NULL;
		length = 0;
	}
};

List<allocated_memory> mem;

void mem_free(allocated_memory* &ptr)
{
	STARTNA(__LINE__, 0)
	if(ptr->memory) delete ptr->memory;
	if(ptr) delete ptr;
	ptr = NULL;
	RETURNV
}

int mem_add(const char* id, const unsigned int length) // return 1 if failed
{
	STARTNA(__LINE__, 0)
	if(mem.find(id)) { RETURN(1) }
	allocated_memory* ptr = new allocated_memory;
	ptr->memory = new Byte[length];
	ptr->length = length;
	mem.push_front(id,ptr);
	RETURN(0)
}

allocated_memory* mem_access(const char* id)
{
	STARTNA(__LINE__, 0)
	allocated_memory* ptr = mem.find(id);
	RETURN(ptr)
}

void mem_clean()
{
	STARTNA(__LINE__, 0)
	const unsigned int size = mem.size();
	for(unsigned int i = 0; i < size; ++i)
	{
		allocated_memory* ptr = mem.front();
		mem.pop_front();
		mem_free(ptr);
	}
	RETURNV;
}

// only during saving data
int mem_save()
{
	STARTNA(__LINE__, 0)
	if(Saver("memo",4)) { RETURN(1); }

	// Saving length of list
	unsigned int size = mem.size();
	if(Saver(&size,sizeof(size))) { RETURN(1) }
	// Saving all entries 
	for(unsigned int i = 0; i < size; ++i)
	{
		allocated_memory* ptr = mem.front();
		char id_buf[ID_LENGTH];
		sprintf_s(id_buf,ID_LENGTH,"%s",mem.id_front());
		if(Saver(id_buf,sizeof(id_buf))) { RETURN(1); }
		mem.pop_front();
		if(Saver(&ptr->length,sizeof(ptr->length))) { RETURN(1); }
		if(Saver(ptr->memory,ptr->length)) { RETURN(1); }
		mem_free(ptr);
	}

	if(Saver("omem",4)) { RETURN(1); }
	RETURN(0)
}

// only during loading data
int mem_load()
{
	STARTNA(__LINE__, 0)
	char head_buf[4] = "";

	if(Loader(&head_buf,4)) RETURN(1);
	if(head_buf[0] != 'm' || head_buf[1] != 'e' || head_buf[2] != 'm' || head_buf[3] != 'o') { MError("Malformed savefile - failed to load allocated memory header"); RETURN(1); }

	// Loading number of entries
	unsigned int size;
	if(Loader(&size,sizeof(size))) { RETURN(1) }
	// Loading entries
	for(unsigned int i = 0; i < size; ++i)
	{
		char id_buf[ID_LENGTH];
		if(Loader(id_buf,sizeof(id_buf))) { RETURN(1); }
		unsigned int length;
		if(Loader(&length,sizeof(length))) { RETURN(1); }
		mem_add(id_buf,length);
		allocated_memory* ptr = mem_access(id_buf);
		if(Loader(ptr->memory,length)) { RETURN(1); }
	}

	if(Loader(&head_buf,4)) RETURN(1);
	if(head_buf[0] != 'o' || head_buf[1] != 'm' || head_buf[2] != 'e' || head_buf[3] != 'm') { MError("Malformed savefile - failed to load allocated memory header"); RETURN(1); }

	RETURN(0)
}

///////////////////////////////////////////////////////////////////////////////////////////////////

// cool but remember it works ONLY with static address
struct patch {
	Byte* to; // address where the data will be copied
	Byte* from; // address from which data will be copied
	Byte* prev_val; // backup of data, to reverse patching later
	unsigned int length; // length of data in bytes

	patch() { 
		to = NULL; 
		from = NULL; 
		prev_val = NULL; 
		length = 0; 
	}
};

List<patch> patches;

void patch_apply(patch* ptr) // prev_val must be initialised
{
	STARTNA(__LINE__, 0) 
	memcpy(ptr->prev_val,ptr->to,ptr->length);
	memcpy(ptr->to,ptr->from,ptr->length);
	RETURNV
}

void patch_reverse(patch* ptr)
{
	STARTNA(__LINE__, 0)
	memcpy(ptr->to,ptr->prev_val,ptr->length);
	RETURNV;
}

int patch_save()
{
	STARTNA(__LINE__, 0)

	RETURN(0)
}

int patch_load()
{
	STARTNA(__LINE__, 0)

	RETURN(0)
}

patch* patch_add(Byte* to, Byte* from, unsigned int length)
{
	STARTNA(__LINE__, 0)
	// Creating id
	char id_buf[ID_LENGTH];
	sprintf_s(id_buf,ID_LENGTH,"%d%d",to,patches.size());
	// Adding new entry
	patch* ptr = new patch;
	ptr->to = to;
	ptr->from = from;
	ptr->length = length;
	ptr->prev_val = new Byte[length];
	patches.push_front(id_buf,ptr);
	RETURN(ptr)
}	

void patch_clean()
{
	STARTNA(__LINE__, 0)
	unsigned int size = patches.size();
	for(unsigned int i = 0; i < size; ++i)
	{
		patch* ptr = patches.front();
		patches.pop_front();
		patch_reverse(ptr);
		delete ptr->prev_val; // ???
	}
	RETURNV;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int ERM_Memory(char Cmd,int Num,_ToDo_* sp,Mes *Mp)
{
	STARTNA(__LINE__, 0)

	RETURN(1);
}