#pragma once

#include "../structs.h"

// Structure is send byte after byte. Pointers aren't allowed, though you can use static arrays
// You can add variables here to be automatically saved in savefile. 
// Should work in battles. Assuming that "sending savefile" works just like save/load, then should work in multiplayer aswell. Brief tests suggest that aswell
extern struct _LegacyGenericData_ {
	int val1;
	int val2;
	int val3;
} LegacyGenericData; 

// Send in remote battle
void SendLegacyData(int* len, Byte** buf);
void ReceiveLegacyData(int len, Byte* buf);

// Savefile
int SaveLegacyData();
int LoadLegacyData();

// ERM
int ERM_CurseSetup(char Cmd,int Num,_ToDo_* sp,Mes *Mp);
int ERM_Testing(char Cmd,int Num,_ToDo_* sp,Mes *Mp);