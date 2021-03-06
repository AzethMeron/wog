#pragma once

#include "../structs.h"

// Curses & Blessings 
#define CURSENUM (1000) // Maximum amount of Curses that can be applied to all heroes (combined) at one time
#define CURSETYPE_NUM (100+1) // Maximum amount of CurseTypes (Type 0 is dummy. so effectively it's indexed from 1)
#define CURSE_BLOCKS 200 // number of entries in object-blocking array
extern struct _CurseType_ {
	char PicName[64];
	char Desc[256];
} CurseType[CURSETYPE_NUM]; // index in table = Type
extern int DHVC_Table[CURSE_BLOCKS][3];
////////////////////////////

////////////////////////////////////
//#define GODMONTSTRT   (150+9+1) // после привидения
//#define GODMONTNUM     4        // количество богов
extern struct _GodCurse_{
	int  Type;     // тип проклятия, 0=свободно, "Type of curse, 0=free"
	int  HeroInd;  // герой-хозяин, "hero-host"
	int  StartDay; // день, когда появился у героя, "the day curse was applied"
	int  Length;   // длительность действия, "Duration of curse"
	int  CurseVal; // Значение параметра наказания, "Power of curse"
	int  Backup[2];
} CurseInfo[CURSENUM];

// Curse/blessing Dialog
void BlessesDescr(_MouseStr_ *ms, _Hero_ *hp);

// Check/add curse
int DoesHeroHas(int hn,int type);
int DoesHeroHasVisitCurse(int hn, int type,int stype);
int AddCurse(int cr,int val,int len,int flag,int hi);

// Daily incrementation 
void DaylyCurse(int Owner);

// Saving/Loading
int SaveCursesData();
int LoadCursesData(int);
void ResetCursesData();

// Sphinx
void ApplySphinx(int GM_ai,_Hero_ *Hr,_MapItem_ * /*Mi*/);

// ERM receivers
int ERM_CurseSetup(char Cmd,int Num,_ToDo_* sp,Mes *Mp); // CR
int ERM_Curse(Mes &M, int Num, int hn); // HE:Y