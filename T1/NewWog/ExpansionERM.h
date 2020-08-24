#pragma once

#include "../structs.h"

// No pointers/arrays allowed here
// You can add variables here to be automatically saved in savefile. Don't know if it works with multiplayer. Doesn't support battles yet - that's for certain
// Actual object is created in erm.cpp
extern struct DataToBeSaved {
	int val1;
	int val2;
	int val3;
} WogLegacyData; 
