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

struct allocated_memory {
	Byte* memory; // address in memory
	unsigned int length; // length in bytes

	allocated_memory() {
		memory = NULL;
		length = 0;
	}
};

List<allocated_memory> mem;

