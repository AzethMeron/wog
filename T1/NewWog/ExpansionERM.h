#pragma once

#include "../structs.h"
#include "../erm_lua.h"

// Creatures
void AddSkeletonPatch(int cr_id, int cr_new);
void RemoveSkeletonPatch(int cr_id, bool error_on_absence = false);

// Savefile
int SaveLegacyData();
int LoadLegacyData();
void ResetLegacyData();

// ERM
int ERM_Testing(char Cmd,int Num,_ToDo_* sp,Mes *Mp);
int ERM_BlackMarket(char Cmd,int Num,_ToDo_* sp,Mes *Mp);
int ERM_VarList(char Cmd,int Num,_ToDo_* sp,Mes *Mp);
//int ERM_Signal(char Cmd,int Num,_ToDo_* sp,Mes *Mp);

