#pragma once

#include "structs.h"
struct _HeroSpecCus_{
	int   Used;
	char  HPSName[24];
	char  HPLName[24];
	Byte *HPSLoaded;
	Byte *HPLLoaded;
//  char *path;
};
extern _HeroSpecCus_ HeroSpecCus[];
#define LockGroups(n) (*(int*)(0x694C5C + 8*n) - 1)

void SetHeroPic(int hn,char *Lpic,char *Spic,char *path);
void ChangeHeroPic(int hn,char *Lpic,char *Spic);
void ChangeHeroPicN(int hn,int hn1);
void PrepareSpecWoG(int WoG);
void __stdcall SpecPicture(Dword po);
void __stdcall SpecPictureS(Dword,Dword,Dword,Dword,Dword,
														Dword,Dword,Dword,Dword,Dword,Dword);
void __stdcall SpecPictureC(Byte *,char *,Dword d3,int,char *,Dword d0);
//void SpecText(char *buf,char *po);
void MapSetup(void);
void NextHeroChoosen(void);
// 3.58
Dword CarryOverHero(void);
void FixBioBug(void);
/////////////////////////
void HideArea(int X,int Y,int Level,int Owner,int Radius);
int CalcHideArea(_Hero_ *Hp,int Correct);
void __stdcall HeroCheck(int NewX,int NewY,int Level,int Owner,int Radius,int Flag);
void MakeDarkness(int Owner);
int NewSpecPrepare(void);
int NewSpecPrepare2(void);
int SaveHeroData(void);
int LoadHeroData(int ver);
void ResetHeroData(void);
void NewSpecShow(void);
void PutBackHeroInfo(void);
void RefreshHeroPic(void);
void InitHeroLocal(void);
// 3.58
int ChooseArt(_Hero_ *hp,int Remove);