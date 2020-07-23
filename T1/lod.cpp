#include <string.h>
#include <stdio.h>
#include "structs.h"
#include "common.h"
#include "txtfile.h"
#include "string.h"
#include "service.h"
#include "erm.h"
#include "b1.h"
#include "lod.h"

#define __FILENUM__ 19

LodTypes::_Table LodTypes::Table[4][3]={
	{{2,Seq[0][0]},{2,Seq[0][1]},{2,Seq[0][2]}},
	{{3,Seq[1][0]},{3,Seq[1][1]},{3,Seq[1][2]}},
	{{1,Seq[2][0]},{1,Seq[2][1]},{2,Seq[2][2]}},
	{{2,Seq[3][0]},{2,Seq[3][1]},{2,Seq[3][2]}}
};
// 1 def
// 2 txt,pic
// 3 wav
int LodTypes::Seq[4][3][LODNUM]={
	{{5,1},{4,0},{1,0}},
	{{7,3,1},{6,2,0},{2,1,0}},
	{{1},{0},{1,0}},
	{{1,3},{0,2},{1,0}}
};

int FreeInd=8;
int NextInd[LODNUM];
Byte LodTable[LODNUM][0x190];

#define SOD_LOD_ctor(lod,LODFilename) (((int (__fastcall *)(Byte*,int,char *))0x559420)(lod,0,LODFilename))
#define SOD_LOD_dtor(lod,LODFilename) (((int (__fastcall *)(Byte*,int,char *))0x559460)(lod,0,LODFilename))

#define SOD_SearchFileFromLOD(lod,FilenameToSearch) (((char (__fastcall *)(Byte*,int,char *))0x4FB100)(&lod[4],0,FilenameToSearch))
#define SOD_LoadFileFromLOD(lod,FilenameToLoad) (((Byte *(__fastcall *)(Byte*,int,char *))0x4FACA0)(&lod[4],0,FilenameToLoad))

#define SOD_BinTree (0x69E560)
#define SOD_BinTree_FindItem(Name) (((int (__fastcall *)(Dword,int,char *))0x55EE00)(SOD_BinTree,0,Name))
#define SOD_BinTree_AddItem(buf, Name) (*((int *(__fastcall *)(Dword,int,void*,char *))0x55DDF0)(SOD_BinTree,0,(void*)buf,Name))
#define SOD_BinTree_RemoveItem(buf_po,Item_po) (((int (__fastcall *)(Dword,int,int*,Dword))0x55DF20)(SOD_BinTree,0,buf_po,Item_po))

#define SOD_FindDefLod(name) (((int (__fastcall *)(char*))0x55D160)(name))
#define SOD_FindPcxLod(name) (((int (__fastcall *)(char*))0x55D1F0)(name))

Lod *Lod::First=0;

Lod::Lod(int location, char *name, int kind){
	Ind=-1;
	STARTNA(__LINE__, 0)
	Kind = kind;
	Location=location;
	strncpy(Name,name,31); Name[31]=0;
	Ind=FreeInd;
	memset(LodTable[Ind],0,0x190);
	SOD_LOD_ctor(LodTable[Ind],Name);
	Next=First;
	First=this;
	int ret=LoadIt(name);
	if(ret){ 
		char buf[1024];
		sprintf(buf,"Cannot Load LOD file.\nReason : %i\nName : %s\n\nThe game is unstable now!\nPlease fix the problem and run h3WoG.exe again",
			ret,Name);
		TError(buf);
	} else {
		FreeInd = NextInd[Ind];
		if (FreeInd == 0) FreeInd = Ind + 1;
		ReloadItems();
	}
	RETURNV
}

Lod::~Lod(){
	STARTNA(__LINE__, 0)
	if(Ind<0 || LodTypes::Move2ListEnd(Ind)) RETURNV
	ReloadItems();
	LodTypes::Del4List(Ind);
	NextInd[Ind] = FreeInd;
	FreeInd = Ind;
	SOD_LOD_dtor(LodTable[Ind],Name);
	RETURNV
}

int Lod::LoadIt(char *name){
	STARTNA(__LINE__, 0)
	if(Ind<0) RETURN(1)
	char fullpathname[256];
	strncpy(fullpathname,GetFolder(Location),255); fullpathname[255]=0;
	strcat_s(fullpathname, 256, name); // allow absolute path for temporary LODs
	if(LodTypes::Load(Ind, fullpathname)) RETURN(2)
	int ret=LodTypes::Add2List(Ind);
	RETURN(ret)
}

void Lod::ReloadItems()
{
	STARTNA(__LINE__, 0)
	int lod = (int)&LodTable[Ind][4];
	int n = *(int*)(lod + 0x178);
	char *name = *(char**)(lod + 0x180);
	for(; n > 0; n--, name += 32)
		LodTypes::ReloadItem(name);
	RETURNV
}

static int BinTree_FindItem(char *name)
{
	int ret=SOD_BinTree_FindItem(name);
	if(ret==*(int *)(SOD_BinTree + 4))  return 0;
	if(strcmpi(name,(char *)(ret+0x0C))<0)  return 0;
	return ret;
}

static int buf[347];
static void DoReload(int *a, int *b, int size, char *name)
{
	memcpy(buf, a, size);
	memcpy(a, b, size);
	memcpy(b, buf, size);
	// restore RefCount
	b[6] = a[6];
	a[6] = buf[6];
	// delete temp item
	SOD_Deref(b);
	int ti = SOD_BinTree_AddItem(buf, name);
	*(int**)(ti + 0x1C) = a;
}

void LodTypes::ReloadItem(char *name){
	STARTNA(__LINE__, 0)
	int ti = BinTree_FindItem(name);
	int *item = (ti ? *(int**)(ti + 0x1C) : 0);

	// Reload Def/Pcx
	switch (item ? *item : 0)
	{
		case 0x63D6B0: // def
			if (SOD_FindDefLod(name))
			{
				SOD_BinTree_RemoveItem(&ti, ti);
				DoReload(item, (int*)SOD_LoadDef(name), 0x38, name);
			}
			break;
		case 0x63BA14: // pcx
			if (SOD_FindPcxLod(name))
			{
				SOD_BinTree_RemoveItem(&ti, ti);
				DoReload(item, (int*)SOD_LoadPcx(name), 0x56C, name);
			}
			break;
		case 0x63B9C8: // pcx16
			if (SOD_FindPcxLod(name))
			{
				SOD_BinTree_RemoveItem(&ti, ti);
				DoReload(item, (int*)SOD_LoadPcx16(name), 0x38, name);
			}
			break;
	}
	RETURNV
}

//////////////////////////////
int LodTypes::Add2List(int ind){
	STARTNA(__LINE__, 0)
	if((ind<0)||(ind>=LODNUM)){ TError("Incorrect LOD index to add"); RETURN(-1); }
	for(int i=2;i<4;i++){
		for(int j=0;j<2;j++){
			int  n=Table[i][j].Num; 
			int *t=Table[i][j].Inds;
			if(n>=LODNUM){ Del4List(ind); TError("Too many LODs"); RETURN(-2); }
			for(int k=n;k>0;k--){ t[k]=t[k-1];}
			t[0]=ind; Table[i][j].Num++;
		}
	}
	RETURN(0)  
}
int LodTypes::Move2ListEnd(int ind){
	STARTNA(__LINE__, 0)
	if((ind<0)||(ind>=LODNUM)){ TError("Incorrect LOD index to move"); RETURN(-3); }
	for(int i=2;i<4;i++){
		for(int j=0;j<2;j++){
			int  n=Table[i][j].Num; 
			int *t=Table[i][j].Inds;
			for(int k=0;k<n;k++){ 
				if(t[k]==ind){ // found
					for(int l=k+1;l<n;l++) t[l-1]=t[l];
					t[n-1]=ind;
					break;
				}
			}
		}
	}
	RETURN(0)  
}
int LodTypes::Del4List(int ind){
	STARTNA(__LINE__, 0)
	if((ind<0)||(ind>=LODNUM)){ TError("Incorrect LOD index to del"); RETURN(-3); }
	for(int i=2;i<4;i++){
		for(int j=0;j<2;j++){
			int  n=Table[i][j].Num; 
			int *t=Table[i][j].Inds;
			for(int k=0;k<n;k++){ 
				if(t[k]==ind){ // found
					for(int l=k+1;l<n;l++) t[l-1]=t[l];
					t[n-1]=0;
					Table[i][j].Num--;
					break;
				}
			}
		}
	}
	RETURN(0)  
}

//////////////////////////////
int Lod::LoadCustomLOD(int location, char *name, int kind){
	STARTNA(__LINE__, 0)
	if(name==0){ 
		TError("No name for LOD");
		RETURN(-1)
	}
	if(FreeInd>=LODNUM){ 
		TError("Too many loaded LODs");
		RETURN(-1)
	}
	Lod *lp=new Lod(location, name, kind);
	RETURN(lp->Ind);
}

void Lod::UnloadCustomLOD(int ind){
	STARTNA(__LINE__, 0)
	Lod **lp = &First;
	while(Lod *l = *lp){
		if(l->Ind==ind){
			*lp = l->Next;
			delete l;
			break;
		}
		lp = &l->Next;
	}
	RETURNV
}

//////////////////////////////
int ERM_LODs(char Cmd,int Num,_ToDo_* /*sp*/,Mes *Mp)
{
	STARTNA(__LINE__, 0)
	switch(Cmd){
		case 'L': // 3.59 L$1 [var to store index]/$2/$3[z var with name]
		case 'T': // 3.59 T - temporary LOD, not stored in savegame
		{
			int loc=0,ind=-1;
			Apply(&ind,4,Mp,0);
			char *name="undefined";
			CHECK_ParamsNum(3);
			if(BAD_INDEX_LZ(Mp->n[2])){ MError("\"LD:L\"-wrong z var index (-20...-1,1...1000)."); RETURN(0) }
			if(Mp->n[2]>1000) name=ERM2String(StringSet::GetText(Mp->n[2]),1,0);
			else if(Mp->n[2]>0) name=ERM2String(ERMString[Mp->n[2]-1],1,0);
			else name=ERM2String(ERMLString[-Mp->n[2]-1],1,0);
			Apply(&loc,4,Mp,1);
			if(Cmd == 'L'){
				if(loc==5){ MError2("script folder can't be specified as LOD base location"); RETURN(0) }
				if(loc==10){ MError2("absolute path can't be specified as LOD location"); RETURN(0) }
				if(strlen(name) > 31){ MError2("LOD path must not exceed 31 character"); RETURN(0) }
			}
			ind = Lod::LoadCustomLOD(loc, name, (Cmd == 'L' ? Lod::STORED : Lod::TEMP));
			Apply(&ind,4,Mp,0);
			break;
		}
		case 'U': // 3.59 U$1 [loaded lod index]
		{
			int ind=-1;
			Apply(&ind,4,Mp,0);
			Lod::UnloadCustomLOD(ind);
			break;
		}
		default: EWrongCommand(); RETURN(0)
	}
	RETURN(1)
}

///////////////////////////////

int ResetLODs(void){
	// unload extra LODs
	STARTNA(__LINE__, 0)
	Lod **lp = &Lod::First;
	while(Lod *l = *lp){
		if(l->Kind != Lod::PERM){
			*lp = l->Next;
			delete l;
		}else{
			lp = &l->Next;
		}
	}
	RETURN(0)
}

int LoadLODs(int /*ver*/)
{
	STARTNA(__LINE__, 0)
	ResetLODs();
	char buf[4]; if(Loader(buf,4)) RETURN(1)
	if(buf[0]!='L'||buf[1]!='O'||buf[2]!='D'||buf[3]!='0') {MError("LoadLODs cannot start loading"); RETURN(1)}
	int  num;
	if(Loader(&num,sizeof(int))) RETURN(1)
	if(num==0) RETURN(0)
	struct _lod_{ int l; char n[32]; };
	_lod_ *ar=(_lod_ *) new _lod_[num];
	if(ar==0) RETURN(1)
	for(int i=0;i<num;i++){
		if(Loader(&ar[i].l,sizeof(int))){ delete ar; RETURN(1) }
		if(Loader(ar[i].n,32)){ delete ar; RETURN(1) }
		ar[i].n[31]=0;
	}
	for(int i = num - 1; i > 0; i--){
		Lod::LoadCustomLOD(ar[i].l, ar[i].n, Lod::STORED);
	}
	delete ar;
	RETURN(0)
}

int SaveLODs(void){
	STARTNA(__LINE__, 0)
	if(Saver("LOD0",4)) RETURN(1);
	int Num = 0;
	for (Lod* lod = Lod::First; lod; lod = lod->Next)
		if (lod->Kind == Lod::STORED)
			Num++;
	if(Saver(&Num,sizeof(int))) RETURN(1)
	if(Num==0) RETURN(0)
	for (Lod* lod = Lod::First; lod; lod = lod->Next){
		if (lod->Kind == Lod::STORED){
			if(Saver(&lod->Location,sizeof(int))) RETURN(1)
			if(Saver(&lod->Name,32)) RETURN(1)
		}
	}
	RETURN(0)
}
