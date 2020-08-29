#include "../common.h"
#include "../structs.h"
#include "../b1.h"
#include "../erm.h"
#include "../string.h"
#include "../classes.h"
#include "../service.h"
#include "../monsters.h"
#include "Curse.h"
#include "Curse_Hardcoded.h"

#define __FILENUM__ 30

struct _CurseType_ CurseType[CURSETYPE_NUM]; // Types of curses
int DHVC_Table[CURSE_BLOCKS][3]; // Blocked objets
short int AS_CBad[CURSETYPE_NUM][3]; // Sphinx pool - curses
short int AS_CGood[CURSETYPE_NUM][3]; // Sphinx pool - blessings
_GodCurse_ CurseInfo[CURSENUM]; //  Curses granted to heros

// 
static int FillCurseStruct(_Hero_ *hr);
static char _GC_Length[100][50];
static char *GC_Pics[64]; // Pointers to pictures (paths)
static char *GC_Descr[256]; // Pointers to descriptions
static char *GC_Length[100]; // String containg "length", like this
// 71 "Will last for another %s turns."
// 72 "Will last forever :-)"
/***********************************************************************************/

/*********************** LOCAL FUNCTION - CREATE CURSE DIALOG **********************/
static _CurseShow CurseShow={GC_Pics,GC_Descr,GC_Length};
_ZPrintf_ Descr1;
void BlessesDescr(_MouseStr_ *ms, _Hero_ *hp)
{
	if (ms->Item != 151 && ms->Item != 152)
		return;

	STARTNA(__LINE__, 0)
	char *str;
	if (ms->Item == 151){
		int gt=DoesHeroGot(hp);
		if(gt==0){ 
			str=ITxt(53,0,&Strings);
		}else{
			Zsprintf2(&Descr1,ITxt(54,0,&Strings),(Dword)ITxt(55+gt-GODMONTSTRT,0,&Strings),
								 GetGodBonus(hp->Number,0));
			str=Descr1.Str;
		}
	}
	if (ms->Item == 152){
		int cr=DoesHeroHas(hp->Number,0);
		if(cr==-1 || ms->Flags & 512){ 
			str=ITxt(70,0,&Strings);
		}else{
			FillCurseStruct(hp);
			ShowCurse(&CurseShow);
			RETURNV
		}  
	}
	Message(str, ((ms->Flags & 512) ? 4 : 1));
	RETURNV
}
static _ZPrintf_ FCS_tmp;
static int FillCurseStruct(_Hero_ *hr)
{
	STARTNA(__LINE__, 0)
	int i,j,cnum;
	if(hr==0) RETURN(-1)
	for(i=0,j=0;i<CURSENUM;i++){
		if(CurseInfo[i].Type==0) continue;
		if(CurseInfo[i].HeroInd!=hr->Number) continue;
		cnum=CurseInfo[i].Type;
		if(j>=99){ Error(); RETURN(0) }
		GC_Pics[j] = CurseType[cnum].PicName;
		GC_Descr[j] = CurseType[cnum].Desc;
		if(CurseInfo[i].Length==9999){ // вечно
			StrCopy(_GC_Length[j],50,ITxt(72,0,&Strings));
			GC_Length[j]=_GC_Length[j];
		}else{
			Zsprintf2(&FCS_tmp,ITxt(71,0,&Strings),
				(Dword) CurseInfo[i].CurseVal,
				(Dword)(CurseInfo[i].StartDay+CurseInfo[i].Length-GetCurDate()));
			StrCopy(_GC_Length[j],50,FCS_tmp.Str);
			GC_Length[j]=_GC_Length[j];
		}
		++j;
	}
	GC_Pics[j]=0;
	RETURN(0)
}
/***********************************************************************************/

/********************** OLD FUNCTIONS, MANAGE CURSES ON HEROS **********************/
// ищет
int DoesHeroHas(int hn,int type)
{
	STARTNA(__LINE__, 0)
	for(int i=0;i<CURSENUM;i++){
		if(CurseInfo[i].Type==0) continue;
		if(type!=0){
			if(CurseInfo[i].Type!=type) continue;
		}
		if(CurseInfo[i].HeroInd==hn) RETURN(i)
	}
	RETURN(-1)
}
int FindFreeCurse()
{
	STARTNA(__LINE__, 0)
	for(int i=0;i<CURSENUM;i++){
		if(CurseInfo[i].Type==0) RETURN(i)
	}
	RETURN(-1)
}
int DoesHeroHasVisitCurse(int hn, int type,int stype)
{
	STARTNA(__LINE__, 0)
	int cn;
	if((type==63)&&(stype>0)) RETURN(-1) // ERM object cannot be prohibited
	for(int i=0;;i++){
		cn=DHVC_Table[i][0];
		if(cn==0) break;
		if(DHVC_Table[i][1]==type){
			if((DHVC_Table[i][2]==stype)||(DHVC_Table[i][2]==-1)) goto found;
		}
	}
	RETURN(-1) // не нашли такого типа
found:  
	RETURN(DoesHeroHas(hn,cn))
}
char LockGroupSize[14];
void NeedLockGroupSize()
{
	FillMemory(LockGroupSize, sizeof(LockGroupSize), 0);
	for (int i = 0; i < 19; i++)
		LockGroupSize[LockGroups(i)]++;
}
static char* _AddCurse(int hn,int cr,int *val,int ind)
{
	STARTNA(__LINE__, 0)
	int i,j,n;
	_Hero_ *hp=GetHeroStr(hn);
	_GodCurse_ *cu = &CurseInfo[ind];
	NeedLockGroupSize();
	switch(cr){
		case CURSE_BLIND:
			break;
		case CURSE_SLOCK:
			i = *val;
			if (i == -1)
			{
				for (n = -1, j = 0; j <= 8; j++)
					if (hp->LockedSlot[j] < LockGroupSize[j])
						n++;
				if (n < 0)  RETURN("all slots are already locked.")
				n = Random(0,n);
				for (j = 0; j <= 8; j++)
					if (hp->LockedSlot[j] < LockGroupSize[j] && --n < 0)
						break;
				*val = i = j;
			}
			else
			{
				if (hp->LockedSlot[i] >= LockGroupSize[i])  RETURN("the slot is already locked.")
			}
			// now lock it
			n = ++hp->LockedSlot[i];
			for (j = 18; j >= 0; j--)
				if ((LockGroups(j) == i) && (hp->IArt[j][0] == -1) && (--n <= 0))
					break;

			if (j < 0) // need to remove an art
			{
				j = 0;
				while (LockGroups(j) != i || hp->IArt[j][0] == -1)  j++;
				cu->Backup[0] = hp->IArt[j][0];
				cu->Backup[1] = hp->IArt[j][1];
				hp->IArt[j][0] = hp->IArt[j][1] = -1;
			}
			else
			{
				cu->Backup[0] = cu->Backup[1] = -1;
			}
			break;
	}
	RETURN(0)
}
// Remove curse from hero
static int _DelCurse(int hn,int cr,int ind)
{
	STARTNA(__LINE__, 0)
	_Hero_     *hp = GetHeroStr(hn);
	_GodCurse_ *cu = &CurseInfo[ind];
	int  val = cu->CurseVal;
	int  i,j;

	cu->Type=0;
	cu->HeroInd=0;
	cu->StartDay=0;
	cu->Length=0;
	cu->CurseVal=0;

	switch(cr){
		case CURSE_BLIND:
			ShowArea(hp->x,hp->y,hp->l,hp->Owner,2);
			RedrawMap();
			break;
		case CURSE_SLOCK:
			i = val;
			--hp->LockedSlot[i];
			if (cu->Backup[0] != -1) // return art
			{
				j = 0;
				while (LockGroups(j) != i || hp->IArt[j][0] != -1)
					if (++j > 18){ MError("Curse error - unable to find place for artifact"); RETURN(0) }
				hp->IArt[j][0] = cu->Backup[0];
				hp->IArt[j][1] = cu->Backup[1];
			}
			break;
	}
	RETURN(0)
}
// Add curse to hero
int AddCurse(int cr,int val,int len,int flag,int hi)
{
	STARTNA(__LINE__, 0)
	int i;
	if(hi<0 || hi>=HERNUM) RETURN(0)
	if(flag==3){ // удалить все
		for(i=0;i<CURSENUM;i++){
			cr=CurseInfo[i].Type;
			if(cr==0) continue;
			if(CurseInfo[i].HeroInd!=hi) continue;
			if(_DelCurse(hi,cr,i)){ Error(); RETURN(-1) }// почему-то не удалилось
		}
		RETURN(0)
	}
	if (flag != 2) { MError("assertion failed in AddCurse"); RETURN(-1) }
	for(i=0;i<CURSENUM;i++){
		if(CurseInfo[i].Type!=cr) continue;
		if(CurseInfo[i].HeroInd!=hi) continue;
		CurseInfo[i].Length+=len;
		CurseInfo[i].CurseVal = max(val, CurseInfo[i].CurseVal);
		RETURN(0)
	}  
	for(i=0;i<CURSENUM;i++){
		if(CurseInfo[i].Type!=0) continue;
		if(_AddCurse(hi,cr,&val,i)) RETURN(0) // почему-то не добавилось
		CurseInfo[i].Type=cr;
		CurseInfo[i].HeroInd=hi;
		CurseInfo[i].StartDay=GetCurDate();
		CurseInfo[i].Length=len;
		CurseInfo[i].CurseVal=val;
		RETURN(0)
	}
	RETURN(-1)
}
// Daily effect of curse
void DaylyCurse(int Owner)
{
	STARTNA(__LINE__, 0)
	int     i,day,cr,val,hn,v;
	_Hero_ *hr;
	day=GetCurDate();
	for(i=0;i<CURSENUM;i++){
		cr=CurseInfo[i].Type;
		if(cr==0) continue;
		val=CurseInfo[i].CurseVal;
		hn=CurseInfo[i].HeroInd;
		hr=GetHeroStr(hn);
		if(hr->Owner!=Owner) continue;
		if((CurseInfo[i].StartDay+CurseInfo[i].Length)<day){ // закончилось
			if(_DelCurse(hn,cr,i)){ Error(); RETURNV }// почему-то не удалилось
			continue;
		}
		switch(cr){
			case CURSE_NMONY : AddRes(Owner,6,-val); break;
			case CURSE_NMANA : v=(int)hr->SpPoints-val; if(v<0) v=0; hr->SpPoints=(Word)v; break;
			case CURSE_PMANA : v=(int)hr->SpPoints+val; if(v>900) v=900; hr->SpPoints=(Word)v; break;
			case CURSE_PEXP  : hr->Exp+=val; AddExp(hr); break;
			case CURSE_SLOW  : hr->Movement-=val; break;
			case CURSE_SPEED : hr->Movement+=val; break;
			case CURSE_PR1345: AddRes(Owner,1,val); AddRes(Owner,3,val); AddRes(Owner,4,val); AddRes(Owner,5,val); break;
			case CURSE_PR02  : AddRes(Owner,0,val); AddRes(Owner,2,val); break;
			case CURSE_PR0   : AddRes(Owner,0,val); break; // дерево
			case CURSE_PR2   : AddRes(Owner,2,val); break; // руда
			case CURSE_PR5   : AddRes(Owner,5,val); break; // Самоцветы
			case CURSE_PR1   : AddRes(Owner,1,val); break; // Ртуть
			case CURSE_PR3   : AddRes(Owner,3,val); break; // Сера
			case CURSE_PR4   : AddRes(Owner,4,val); break; // Кристаллы
			case CURSE_PR6   : AddRes(Owner,6,val); break; // Золото
		}
	}
	// бонусы существ
	for(i=0;i<HERNUM;i++){
		hr=GetHeroStr(i);
		if(hr->Owner!=Owner) continue;
		if(CheckForCreature(hr,151)==1){ // алмазный дракон
			AddRes(Owner,5,1);
		}
		MagicWonder(hr);
	}
	RETURNV
}
int LuaGetHeroGod(lua_State *L)
{
	int gt = DoesHeroGot((_Hero_*)lua_tointeger(L, 1));
	if(gt){
		lua_pushinteger(L, gt - GODMONTSTRT);
		return 1;
	}
	return 0;
}
int LuaHeroHasBlessCurse(lua_State *L)
{
	lua_pushboolean(L, DoesHeroHas(((_Hero_*)lua_tointeger(L, 1))->Number, 0) >= 0);
	return 1;
}
/***********************************************************************************/

/******************** LOCAL FUNCTION - MANAGE BLOCKS FOR OBJECT ********************/
int FindCurseBlockObject(int index, int type, int subtype) // -1 means not found
{
	for(int i = 0; i < CURSE_BLOCKS; ++i)
	{
		if(DHVC_Table[i][0] != index) continue;
		if(DHVC_Table[i][1] != type) continue;
		if(DHVC_Table[i][2] != subtype) continue;
		return i;
	}
	return -1;
}
int FindEndOfBlockedObjects() // find first {0,0,0}
{
	for(int i = 0; i < CURSE_BLOCKS; ++i)
	{
		if(DHVC_Table[i][0] != 0) continue;
		return i;
	}
	return -1;
}
int AddCurseBlockObject(int index, int type, int subtype)
{
	int entry = FindCurseBlockObject(index,type,subtype);
	if(entry != -1) return 0;
	entry = FindEndOfBlockedObjects();
	if(entry == -1) return 1;
	DHVC_Table[entry][0] = index;
	DHVC_Table[entry][1] = type;
	DHVC_Table[entry][2] = subtype;
	if(entry < CURSE_BLOCKS-1) 
	{
		DHVC_Table[entry+1][0] = 0;
		DHVC_Table[entry+1][1] = 0;
		DHVC_Table[entry+1][2] = 0;
	}
	return 0;
}
void RemoveCurseBlockObject(int index, int type, int subtype)
{
	int entry = FindCurseBlockObject(index,type,subtype);
	if(entry == -1) return;
	int end = FindEndOfBlockedObjects(); 
	if(end == -1) end = CURSE_BLOCKS;
	for(int i = 0; i < 3; ++i)
	{
		DHVC_Table[entry][i] = DHVC_Table[end-1][i];
		DHVC_Table[end-1][i] = 0;
	}
}
/***********************************************************************************/

/*********************** LOCAL FUNCTION - MANAGE SPHINX POOLS **********************/
int FindBlessSphinx(short int index)
{
	for(int i = 1; i <= AS_CGood[0][0]; ++i)
	{
		if(index==AS_CGood[i][0]) return i;
	}
	return -1;
}
void RemoveBlessSphinx(short int index)
{
	short int bless_index = FindBlessSphinx(index);
	if(bless_index==-1) return; // exit if there is no such bless
	int end = AS_CGood[0][0];
	if(end == 1) { UniversalErrorMessage("Cannot remove all blessings from sphinx database"); return; }
	for(int i = 0; i < 3; ++i)
	{
		AS_CGood[bless_index][i] = AS_CGood[end][i];
		AS_CGood[end][i] = 0;
	}
	--AS_CGood[0][0]; 
}
int AddBlessSphinx(short int index, short int min, short int max)
{
	short int bless_index = FindBlessSphinx(index);
	if(bless_index==-1)
	{
		bless_index = AS_CGood[0][0] + 1;
		if(bless_index >= CURSETYPE_NUM) { return 1; } 
		++AS_CGood[0][0];
	}
	AS_CGood[bless_index][0] = index;
	AS_CGood[bless_index][1] = min;
	if(max >= min) AS_CGood[bless_index][2] = max;
	else { AS_CGood[bless_index][2] = min; return 1; }
	return 0;
}
int FindCurseSphinx(short int index)
{
	for(int i = 1; i <= AS_CBad[0][0]; ++i)
	{
		if(index==AS_CBad[i][0]) return i;
	}
	return -1;
}
void RemoveCurseSphinx(short int index)
{
	short int curse_index = FindCurseSphinx(index);
	if(curse_index==-1) return;
	int end = AS_CBad[0][0];
	if(end == 1) { UniversalErrorMessage("Cannot remove all curses from sphinx database"); return; }
	for(int i = 0; i < 3; ++i)
	{
		AS_CBad[curse_index][i] = AS_CBad[end][i];
		AS_CBad[end][i] = 0;
	}
	--AS_CBad[0][0];
}
int AddCurseSphinx(short int index, short int min, short int max)
{
	short int curse_index = FindCurseSphinx(index);
	if(curse_index==-1)
	{
		curse_index = AS_CBad[0][0] + 1;
		if(curse_index >= CURSETYPE_NUM) { return 1; } 
		++AS_CBad[0][0];
	}
	AS_CBad[curse_index][0] = index;
	AS_CBad[curse_index][1] = min;
	if(max >= min) AS_CBad[curse_index][2] = max;
	else { AS_CBad[curse_index][2] = min; return 1; }
	return 0;
}
/***********************************************************************************/

/*********************************** SAVING DATA ***********************************/
int SaveCursesData()
{
	STARTNA(__LINE__, 0)
	if(Saver("CRSE",4)) RETURN(1)
	if(Saver(CurseType,sizeof(CurseType))) RETURN(1)
	if(Saver(DHVC_Table,sizeof(DHVC_Table))) RETURN(1)
	if(Saver(CurseInfo,sizeof(CurseInfo))) RETURN(1)
	if(Saver(AS_CBad,sizeof(AS_CBad))) RETURN(1)
	if(Saver(AS_CGood,sizeof(AS_CGood))) RETURN(1)
	RETURN(0)
}
int LoadCursesData(int)
{
	STARTNA(__LINE__, 0)
	char buf[4]; if(Loader(buf,4)) RETURN(1)
	if(buf[0]!='C'||buf[1]!='R'||buf[2]!='S'||buf[3]!='E')
			{MError("LoadCuresData cannot start loading"); RETURN(1) }
	if(Loader(CurseType,sizeof(CurseType))) RETURN(1)
	if(Loader(DHVC_Table,sizeof(DHVC_Table))) RETURN(1)
	if(Loader(CurseInfo,sizeof(CurseInfo))) RETURN(1)
	if(Loader(AS_CBad,sizeof(AS_CBad))) RETURN(1)
	if(Loader(AS_CGood,sizeof(AS_CGood))) RETURN(1)
	RETURN(0)
}
void ResetCursesData()
{
	STARTNA(__LINE__, 0)
	for(int i=0;i<CURSENUM;i++){
		CurseInfo[i].Type=0;
		CurseInfo[i].HeroInd=0;
		CurseInfo[i].StartDay=0;
		CurseInfo[i].CurseVal=0;
		CurseInfo[i].Length=0;
	}
	// Reinitialising object blocked by curses
	bool end = false;
	for(int i = 0; i < CURSE_BLOCKS; ++i)
	{
		if(o_DHVC_Table[i][0] == 0) end = true;
		for(int j = 0; j < 3; ++j)
		{
			if(!end) DHVC_Table[i][j] = o_DHVC_Table[i][j];
			else DHVC_Table[i][j] = 0;
		}
	}

	// Reinitialising curse pictures and description
	end = false;
	for(int i = 0; i < CURSETYPE_NUM; ++i)
	{
		if(!strcmp(o_GC_Pics[i],"")) end = true;
		if(!end)
		{
			sprintf_s(CurseType[i].PicName,sizeof(CurseType[i].PicName), "%s", o_GC_Pics[i]);
			if(i>40){
				sprintf_s(CurseType[i].Desc,sizeof(CurseType[i].Desc), "%s", ITxt(90+i,0,&Strings) );
			}else{
				sprintf_s(CurseType[i].Desc,sizeof(CurseType[i].Desc), "%s", ITxt(80+i,0,&Strings) );
			}
		}
		else
		{
			sprintf_s(CurseType[i].PicName,sizeof(CurseType[i].PicName), "");
			sprintf_s(CurseType[i].Desc,sizeof(CurseType[i].Desc), "");
		}
	}

	// Reinitialising sphinx reward/penalty
	bool end_good=false, end_bad=false;
	for(int i = 0; i < CURSETYPE_NUM; ++i)
	{
		if(i > o_AS_CGood[0][0]) end_good=true;
		if(i > o_AS_CBad[0][0]) end_bad=true;
		for(int j = 0; j < 3; ++j)
		{
			if(!end_good) AS_CGood[i][j] = o_AS_CGood[i][j];
			else AS_CGood[i][j] = 0;
			if(!end_bad) AS_CBad[i][j] = o_AS_CBad[i][j];
			else AS_CBad[i][j] = 0;
		}
	}
	RETURNV
}
/******************************************************************************/

/*********************************** SPHINX ***********************************/
#define SPHMOVEPOINTS 0x500
void ApplySphinx(int GM_ai,_Hero_ *Hr,_MapItem_ * /*Mi*/)
{
	STARTNA(__LINE__, 0)
	int num,val,len,P_n;
	num=Random(0,TXTSphinx.sn-1);
	if(Hr->Movement<SPHMOVEPOINTS){ // ĺńňü ńâîáîäíűĺ äâčćĺíč˙ ó Ăĺđî˙
		if(GM_ai){
			Message(ITxt(18,0,&Strings),1);
			RETURNV
		}
	}
	Hr->Movement-=SPHMOVEPOINTS; if(Hr->Movement<0) Hr->Movement=0;
	if(GM_ai){
		if(SphinxReq(num)) P_n=1; else P_n=0;
	}else{
		P_n=1; // AI
//    Mi->SetUp&=0xFFFFFFFE; // visited
	}
	if(P_n){
		num=AS_CGood[0][0]; num=Random(1,num);
		if(AS_CGood[num][1]==AS_CGood[num][2]) val=AS_CGood[num][1];
		else val=Random(AS_CGood[num][1],AS_CGood[num][2]);
		num=AS_CGood[num][0];
	}else{
		num=AS_CBad[0][0]; num=Random(1,num);
		if(AS_CBad[num][1]==AS_CBad[num][2]) val=AS_CBad[num][1];
		else val=Random(AS_CBad[num][1],AS_CBad[num][2]);
		num=AS_CBad[num][0];
	}
	len=Random(1,7);
	if(AddCurse(num,val,len,2,Hr->Number)){ Error(); RETURNV }
	if(GM_ai){
		if(P_n){
			Message(ITxt(190,0,&Strings),1);
		}else{
			Message(ITxt(191,0,&Strings),1);
		}
	}
	RedrawMap();
	RETURNV
}
/******************************************************************************/

/******************************* ERM GRANT CURSE ******************************/
int ERM_Curse(Mes &M, int Num, int hn) // HE:Y
{
	STARTNA(__LINE__, 0)
	if(Num<3){ EWrongParamsNum(); RETURN(1) }
	if(Num<4)  M.n[3] = 1;
	if(M.n[3]<0 || M.n[3]>3){ MError2("wrong action kind (0...3)."); RETURN(1) }
	if(M.n[3]==3 && !M.VarI[1].Check && !M.VarI[2].Check)
		RETURN(AddCurse(0, 0, 0, 3, hn));

	int cr = M.n[0]; if(cr < 1 || cr >= CURSETYPE_NUM) { MError2("incorrect curse/bless number"); RETURN(1); }
	int i = DoesHeroHas(hn, cr);
	if (i < 0)
	{
		i = FindFreeCurse();
		if (i < 0) { MError2("too many curses."); RETURN(1) }
	}
	
	int val = CurseInfo[i].CurseVal, len = max(CurseInfo[i].StartDay + CurseInfo[i].Length - GetCurDate(), 0);
	int baseLen = len;
	if (M.n[3] == 2)  // for backward compatibility with possible strange scripts
	{
		if(M.f[1]) val = 0;
		if(M.f[2]) len = 0;
	}
	if (Apply(&val, 4, &M, 1) | Apply(&len, 4, &M, 2))  RETURN(0)  // get syntax
	
	if (M.n[3] == 2)  // add/sub
	{
		len += baseLen;
		val += CurseInfo[i].CurseVal;
	}
	if (M.n[3] == 0 || len <= 0)  // delete
	{
		if ((CurseInfo[i].Length != 0) && _DelCurse(hn, cr, i)) { Error(); RETURN(1) }
		RETURN(0)
	}
	if (cr == CURSE_SLOCK && (val < -1 || val > 13)){ MError2("wrong slot number (-1...13)"); RETURN(1) }
	if (cr == CURSE_SLOCK && val != CurseInfo[i].CurseVal && CurseInfo[i].Length != 0)
		if (_DelCurse(hn, cr, i))
			{	Error(); RETURN(1) }

	if (CurseInfo[i].Length == 0)
	{
		char* err = _AddCurse(hn, cr, &val, i);
		if (err){ MError2(err); RETURN(1) }
	}
	CurseInfo[i].Type = cr;
	CurseInfo[i].HeroInd = hn;
	CurseInfo[i].CurseVal = val;
	CurseInfo[i].Length = len;
	CurseInfo[i].StartDay = GetCurDate();
	RETURN(0)
}
/******************************************************************************/

/******************************* ERM SETUP CURSE ******************************/
int ERM_CurseSetup(char Cmd,int Num,_ToDo_* sp,Mes *Mp)
{
	STARTNA(__LINE__, 0)
	switch(Cmd)
	{
		case 'P': // Get/set picture
		{
			int index = GetVarVal(&sp->Par[0]);
			if(index < 1 || index >= CURSETYPE_NUM){ MError2("incorrect curse/blessing index(type)."); RETURN(0) }
			CHECK_ParamsNum(1);
			StrMan::Apply(CurseType[index].PicName,Mp,0,sizeof(CurseType[index].PicName));
		} break;

		case 'D': // Get/set description
		{
			int index = GetVarVal(&sp->Par[0]);
			if(index < 1 || index >= CURSETYPE_NUM){ MError2("incorrect curse/blessing index(type)."); RETURN(0) }
			CHECK_ParamsNum(1);
			StrMan::Apply(CurseType[index].Desc,Mp,0, sizeof(CurseType[index].Desc));
		} break;

		case 'B': // Forbid entering objects
		{
			int index = GetVarVal(&sp->Par[0]);
			if(index < 1 || index >= CURSETYPE_NUM){ MError2("incorrect curse/blessing index(type)."); RETURN(0) }
			CHECK_ParamsNum(3);
			int type=0, subtype=0;
			if(Apply(&type,sizeof(type),Mp,0)) { MError2("Cannot get parameter: Type"); RETURN(0); }
			if(Apply(&subtype,sizeof(type),Mp,1)) { MError2("Cannot get parameter: Subtype"); RETURN(0); }
			if(type==63 && subtype != 0) { MError2("Cannot forbid entrance to WoG objects"); RETURN(0); } 
			int disabled = 0; if(FindCurseBlockObject(index,type,subtype)!=-1) disabled = 1;
			if(!Apply(&disabled,sizeof(disabled),Mp,2))
			{
				if(disabled == 1)
				{
					if(AddCurseBlockObject(index,type,subtype)) { MError2("Too many entries in objects-blocked-by-curse array"); RETURN(0);}
				} else if(disabled == 0) {
					RemoveCurseBlockObject(index,type,subtype);
				} else { MError2("unknown operation:"); RETURN(0);}
			}
		} break;

		case 'F': // Find free cursetype
		{
			Message(Random(1,1));
			CHECK_ParamsNum(1);
			int found = -1;
			for(int i = 1; i < CURSETYPE_NUM; ++i)
			{ if(!strcmp(CurseType[i].PicName,"")) if(!strcmp(CurseType[i].Desc,"")) { found = i; break; } }
			if(Apply(&found,sizeof(found),Mp,0) == 0) { MError2("Cannot set output value"); RETURN(0); }
		} break;

		case 'R': // Get random curse/bless
		{
			CHECK_ParamsMin(2);
			int bless = 0;
			Apply(&bless,sizeof(bless),Mp,0);
			int rand_type = -1;
			int rand_val = -1;
			if(bless == 1) // bless
			{
				int maxnum = AS_CGood[0][0];
				int rand_ind = 1; if(maxnum > 1) rand_ind = Random(1,maxnum);
				rand_type = AS_CGood[rand_ind][0];
				if(AS_CGood[rand_ind][1] == AS_CGood[rand_ind][2]) rand_val = AS_CGood[rand_ind][1];
				else rand_val = Random(AS_CGood[rand_ind][1],AS_CGood[rand_ind][2]);
			}
			else if(bless == 0)
			{
				int maxnum = AS_CBad[0][0];
				int rand_ind = 1; if(maxnum > 1) rand_ind = Random(1,maxnum);
				rand_type = AS_CBad[rand_ind][0];
				if(AS_CBad[rand_ind][1] == AS_CBad[rand_ind][2]) rand_val = AS_CBad[rand_ind][1];
				else rand_val = Random(AS_CBad[rand_ind][1],AS_CBad[rand_ind][2]);
			}
			else
			{
				MError2("Invalid parameter value: bless/curse"); RETURN(0);
			}
			if(Apply(&rand_type,sizeof(rand_type),Mp,1) == 0) { MError2("Cannot set output value"); RETURN(0); }
			if(Num==3) if(Apply(&rand_val,sizeof(rand_val),Mp,2) == 0) { MError2("Cannot set output value"); RETURN(0); }
		} break;

		case 'S': // Sphinx support
		{
			int index = GetVarVal(&sp->Par[0]);
			if(index < 1 || index >= CURSETYPE_NUM){ MError2("incorrect curse/blessing index(type)."); RETURN(0) }
			int bless = -1;
			if(Apply(&bless,sizeof(bless),Mp,0)) { MError2("Cannot get parameter: bless or curse"); RETURN(0); }
			switch(bless)
			{
				case 1: // bless
				{
					int command = 0; if(FindBlessSphinx(index)!=-1) command = 1;
					Apply(&command,sizeof(command),Mp,1);
					if(command==0) { RemoveBlessSphinx(index); }
					if(command==1) if(Mp->VarI[1].Check!=1) if(Num!=4) { MError2("Cannot add blessing without specified Min and Max value"); RETURN(0); };
					if(Num!=4 || command==0) break;
					short int min=0, max=0;
					if(Mp->VarI[2].Check==1 && Mp->VarI[3].Check==1)
					{
						int ind = FindBlessSphinx(index);
						if(ind!= -1) { min = AS_CGood[ind][1]; max = AS_CGood[ind][2]; } 
						if(Apply(&min,sizeof(min),Mp,2)==0) { MError2("Cannot mix get and set syntax"); RETURN(0); }
						if(Apply(&max,sizeof(max),Mp,3)==0) { MError2("Cannot mix get and set syntax"); RETURN(0); }
					}
					else
					{
						if(Apply(&min,sizeof(min),Mp,2)) { MError2("Cannot get parameter: minimum value"); RETURN(0); }
						if(Apply(&max,sizeof(max),Mp,3)) { MError2("Cannot get parameter: maximum value"); RETURN(0); }
						if(max < min) { MError2("Maximum value CANNOT be lower than Minimum"); RETURN(0); }
						if(command==1) { if(AddBlessSphinx(index,min,max)) { MError2("Cannot add more blessings for sphinx"); RETURN(0); } }
					}
				} break;

				case 0: // curse
				{
					int command = 0; if(FindCurseSphinx(index)!=-1) command = 1;
					Apply(&command,sizeof(command),Mp,1);
					if(command==0) { RemoveCurseSphinx(index); }
					if(command==1) if(Mp->VarI[1].Check!=1) if(Num!=4) { MError2("Cannot add curse without specified Min and Max value"); RETURN(0); };
					if(Num!=4 || command==0) break;
					short int min=0, max=0;
					if(Mp->VarI[2].Check==1 && Mp->VarI[3].Check==1)
					{
						int ind = FindCurseSphinx(index);
						if(ind!= -1) { min = AS_CBad[ind][1]; max = AS_CBad[ind][2]; } 
						if(Apply(&min,sizeof(min),Mp,2)==0) { MError2("Cannot mix get and set syntax"); RETURN(0); }
						if(Apply(&max,sizeof(max),Mp,3)==0) { MError2("Cannot mix get and set syntax"); RETURN(0); }
					}
					else
					{
						if(Apply(&min,sizeof(min),Mp,2)) { MError2("Cannot get parameter: minimum value"); RETURN(0); }
						if(Apply(&max,sizeof(max),Mp,3)) { MError2("Cannot get parameter: maximum value"); RETURN(0); }
						if(max < min) { MError2("Maximum value CANNOT be lower than Minimum"); RETURN(0); }
						if(command==1) { if(AddCurseSphinx(index,min,max)) { MError2("Cannot add more blessings for sphinx"); RETURN(0); } }
					}
				} break;
			
				default:
				{
					MError2("Invalid parameter: bless or curse"); RETURN(0); 
				} break;
			}
		} break;

		case 'X': // Log Curses/Blesses
		{
			CHECK_ParamsNum(2);
			char filename[64];
			if(StrMan::Apply(filename,Mp,0,256) == 0) { MError2("Cannot get parameter: logfile name"); RETURN(0);}
			int skip_empty_entries = 1;
			if(Apply(&skip_empty_entries,sizeof(skip_empty_entries),Mp,1)) { MError2("Cannot get parameter: skip empty entries"); RETURN(0); }
			FILE* file = fopen(filename,"at");
			if(file == NULL) { MError2("Cannot open logfile"); RETURN(0);}
			else
			{
				fprintf(file, "Logging all initialised Curses/Blesses\n");
				for(int i = 1; i < CURSETYPE_NUM; ++i)
				{
					if(skip_empty_entries) if(!strcmp(CurseType[i].PicName,"")) continue;
					fprintf(file,"Type %d, PictureName '%s', Description '%s'\n",i,CurseType[i].PicName,CurseType[i].Desc);
				}
				fprintf(file, "Logging all entries in Objects blocked by Curses\n");
				for(int i = 0; i < CURSE_BLOCKS; ++i)
				{
					if(skip_empty_entries) if(DHVC_Table[i][0]==0) continue;
					fprintf(file,"Curse index %d, Object type %d, Object subtype %d\n",DHVC_Table[i][0],DHVC_Table[i][1],DHVC_Table[i][2]);
				}
				fprintf(file, "Logging all entries in Sphinx Curses\n");
				for(int i = 1; i < CURSETYPE_NUM; ++i)
				{
					if(skip_empty_entries) if(AS_CBad[i][0]==0) continue;
					fprintf(file,"Curse index %d, Min %d, Max %d\n",AS_CBad[i][0],AS_CBad[i][1],AS_CBad[i][2]);
				}
				fprintf(file, "Logging all entries in Sphinx Blessings\n");
				for(int i = 1; i < CURSETYPE_NUM; ++i)
				{
					if(skip_empty_entries) if(AS_CGood[i][0]==0) continue;
					fprintf(file,"Bless index %d, Min %d, Max %d\n",AS_CGood[i][0],AS_CGood[i][1],AS_CGood[i][2]);
				}
				fclose(file);
			}
		} break;

		default:
			{ EWrongCommand(); RETURN(0); }
			break;
	}
	RETURN(1);
}
/******************************************************************************/