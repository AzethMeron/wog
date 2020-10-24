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

int mem_save();
int mem_load();
int mem_add(const char* id, const unsigned int length);
allocated_memory* mem_access(const char* id);
void mem_clean();

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

void patch_apply(patch*);
void patch_reverse(patch*);

int patch_save();
int patch_load();
int patch_add(Byte* to, Byte* from, unsigned int length);
void patch_clean();