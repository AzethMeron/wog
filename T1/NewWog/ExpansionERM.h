#pragma once

#include "../structs.h"

// Structure is send byte after byte. Pointers aren't allowed, though you can use static arrays
// You can add variables here to be automatically saved in savefile. 
// Should work in battles. Assuming that "sending savefile" works just like save/load, then should work in multiplayer aswell. Brief tests suggest that aswell
extern struct DataToBeSaved {
	int val1;
	int val2;
	int val3;
} WogLegacyData; 
