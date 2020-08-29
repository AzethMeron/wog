#include "structs.h"
#include "common.h"
#include "txtfile.h"
#include "casdem.h"
#include "service.h"
#include "monsters.h"
#include "herospec.h"
#include "erm.h"
#include "npc.h"
//#include "classes.h"
#include "b1.h"
#include "CrExpo.h"
#include "NewWog/Curse.h"
#define  __FILENUM__ 2

struct _HeroSpecWoG_{
	int    Ind; // +1
	int    SpecPicInd[2];
//  char  *HPSName[2];
//  char  *HPLName[2];
	char  *Name[2];
	char  *HBios[2];
	Byte  *HPSLoaded[2];
	Byte  *HPLLoaded[2];
	_HeroSpec_ Spec[2];
} HeroSpecWoG[]={
// Нагаш
{79,{156},/*{"HPSZ0001.PCX","HPS079Nc.PCX"},{"HPLZ0001.PCX","HPL079Nc.PCX"},*/
		{"1"},{"2"},{0},{0},{{HSUPERSPEC,HSPEC_DARKNESS,0,0,0,0,0,"3","4","5"},{0}}},
// Жеддит
{91,{157},/*{"HPL091WL.PCX","HPS091WL.PCX"},{"HPL091WL.PCX","HPL091WL.PCX"},*/
	 {"6"},{"7"},{0},{0},{{HSUPERSPEC,HSPEC_ANYTOWN,0,0,0,0,0,"8","9","10"},{0}}},
{-1}
};

void InitHeroLocal(void)
{
	STARTNA(__LINE__, 0)
	if(WoGType){
		HeroSpecWoG[0].Name[0]="Коптилка";
		HeroSpecWoG[0].HBios[0]="В детстве Нагаш любил часами сидеть под черной тенью Вуали Тьмы. Рожденный некромантом, он не любил общества и всегда пытался скрыться с глаз. Поэтому, Вуаль Тьмы притягивала его как магнит.  И, конечно, ему было интересно узнать, как она работает. Яблок на голову Нагаша не падало, но терпение и труд сделали свое дело. Долгие годы тренировок позволили ему в совершенстве овладеть такой полезной специальностью, как Вуаль Тьмы. И теперь он не упускает шанса воспользоваться своим навыком.";
		HeroSpecWoG[0].Spec[0].SpShort="Затемнение";
		HeroSpecWoG[0].Spec[0].SpFull="Ходячая Вуаль Тьмы";
		HeroSpecWoG[0].Spec[0].SpDescr="{Передвижная вуаль тьмы}\n\n По мере передвижения Героя по карте, закрывает прилегающую часть поверхности для всех игроков, которые не являются союзниками данного."
			"\n\nРадиус закрываемой поверхности определяется по следующей формуле:"
			"\n1. Выбирается меньшее из полного текущего количества очков заклинаний и уровнем знаний*10."
			"\n2. Делим полученное число на 10 и прибавляем 0.5"
			"\n3. Минимальный радиус 1.5";
		HeroSpecWoG[1].Name[0]="Строитель";
		HeroSpecWoG[1].HBios[0]="В юности Джедит объездил весь мир,  и везде его поражала красота незнакомых городов. Будучи очень любопытным и наблюдательным, он всегда расспрашивал местных каменщиков о том, как же такие города строятся. Теперь, разобравшись в этом, он смог бы, пожалуй, построить любой город.";
		HeroSpecWoG[1].Spec[0].SpShort="Постройка";
		HeroSpecWoG[1].Spec[0].SpFull="Постройка Города";
		HeroSpecWoG[1].Spec[0].SpDescr="Позволяет выбирать любой тип города при восстановлении полностью разрушенного города.";
	}else{
		HeroSpecWoG[0].Name[0]="Pitch";
		HeroSpecWoG[0].HBios[0]="In childhood, Nagash liked to sit for hours under the black shadow of the Veil of Darkness. Because he was a necromancer, he did not like society and always tried to disappear from sight. And thus, the Veil of Darkness drew him like a magnet. He was intrigued by how it functioned, and after much study he finally understood. In essence, it was extremely simple and only required a source of mana to be channeled. Long years of training have allowed Nagash to perfect this useful ability and now he wastes no time in taking advantage of the skill.";
		HeroSpecWoG[0].Spec[0].SpShort="Darkness";
		HeroSpecWoG[0].Spec[0].SpFull="Pitch darkness";
		HeroSpecWoG[0].Spec[0].SpDescr="{Mobile Veil of Darkness}\n\n When the hero moves, the shroud is replaced in a circle around him for all players who are not allies."
			"\n\nRadius of the shroud circle is determined by the following formula:"
			"\n1. Take the lesser of current spell points or (knowledge x 10)"
			"\n2. This number is divided by 10 and then 0.5 is added"
			"\n3. Minimum radius is 1.5";
		HeroSpecWoG[1].Name[0]="Builder";
		HeroSpecWoG[1].HBios[0]="In youth, Jeddite traveled all over the world, and everywhere he went he was amazed at the beauty of unfamiliar cities. Being very curious and observant, he always asked local masons how such cities were constructed. Now, having this knowledge, he is able to direct the construction of  any type of city that is being rebuilt.";
		HeroSpecWoG[1].Spec[0].SpShort="Rebulding";
		HeroSpecWoG[1].Spec[0].SpFull="Town Rebuilding";
		HeroSpecWoG[1].Spec[0].SpDescr="Allows Hero to choose any type of city when rebuilding one that has been completely destroyed.";
	}
	RETURNV
}

struct _HeroSpecCus_ HeroSpecCus[HERNUM];
int HeroInfoBackStored=0;
_HeroInfo_    HeroInfoBack[HERNUM+8]; //?????????
struct _PicsBack_{
	Byte *HPSLoaded;
	Byte *HPLLoaded;
} PicsBack[HERNUM];

void PutBackHeroInfo(void)
{
	STARTNA(__LINE__, 0)
	if(HeroInfoBackStored==0) RETURNV
	_HeroInfo_ *hp=GetHIBase();
	Copy((Byte *)HeroInfoBack,(Byte *)hp,sizeof(HeroInfoBack));
	RETURNV
}

void SetHeroPic(int hn,char *Lpic,char *Spic, char * /*path*/)
{
	STARTNA(__LINE__,Lpic)
	_HeroSpecCus_ *p;

	if((hn<0)||(hn>=HERNUM)){ EWrongParam(); RETURNV }
	p=&HeroSpecCus[hn];
	p->Used=1;
	StrCopy(p->HPLName,13,Lpic);
	StrCopy(p->HPSName,13,Spic);
	RETURNV
}

void ChangeHeroPic(int hn,char *Lpic,char *Spic)
{
	STARTNA(__LINE__,Lpic)
	_HeroSpecCus_ *p;
	_HeroInfo_    *hp=GetHIBase();

	if((hn<0)||(hn>=HERNUM)){ EWrongParam(); RETURNV }
	p=&HeroSpecCus[hn];
	if((Lpic==0)&&(Spic==0)){ // восстановить оригинал
		p->Used=0;
		hp[hn].HPLName=HeroInfoBack[hn].HPLName;
//asm int 3
		hp[hn].HPSName=HeroInfoBack[hn].HPSName;
		p->HPLName[0] = 0;
		p->HPSName[0] = 0;
//    RedrawMap();
		RETURNV
	}
	p->Used=1;
	if(Lpic!=0){
		StrCopy(p->HPLName,28,Lpic);
//asm int 3
		hp[hn].HPLName=p->HPLName;
//    p->HPLLoaded=LoadPCX(p->HPLName);
	}
	if(Spic!=0){
		StrCopy(p->HPSName,28,Spic);
//asm int 3
		hp[hn].HPSName=p->HPSName;
//    p->HPSLoaded=LoadPCX(p->HPSName);
	}
//  RedrawMap();
	RETURNV
}

void ChangeHeroPicN(int hn,int hn1)
{
	STARTNA(__LINE__, 0)
	_HeroSpecCus_ *p;
	_HeroInfo_    *hp=GetHIBase();

	if((hn<0)||(hn>=HERNUM)){ EWrongParam(); RETURNV }
	if((hn1<0)||(hn1>=HERNUM)){ EWrongParam(); RETURNV }
	p=&HeroSpecCus[hn];
	if(hn==hn1) p->Used=0; else p->Used=1;
	hp[hn].HPLName=HeroInfoBack[hn1].HPLName;
	StrCopy(p->HPLName,13,hp[hn].HPLName);
//asm int 3
	hp[hn].HPSName=HeroInfoBack[hn1].HPSName;
	StrCopy(p->HPSName,13,hp[hn].HPSName);
//  RedrawMap();
	RETURNV
}

void RefreshHeroPic(void)
{
	STARTNA(__LINE__, 0)
	int i;
	_HeroSpecCus_ *cp;
	_HeroInfo_ *hp=GetHIBase();

	for(i=0;i<HERNUM;i++){
		if(HeroSpecCus[i].Used==0) continue;
		cp=&HeroSpecCus[i];
		if(cp->HPLName[0]!=0){
			hp[i].HPLName=cp->HPLName;
			cp->HPLLoaded=LoadPCX(cp->HPLName);
		}
		if(cp->HPSName[0]!=0){
//asm int 3
			hp[i].HPSName=cp->HPSName;
			cp->HPSLoaded=LoadPCX(cp->HPSName);
		}
	}
	RETURNV
}

void PrepareSpecWoG(int WoG)
{
	STARTNA(__LINE__, 0)
	int   i,j,ind;
//  char *tp,*tp1;
	_HeroSpec_ *sp;
	_HeroSpecWoG_ *wp;
//  _HeroSpecCus_ *cp;
//  char fpath[128];

	_HeroInfo_ *hp=GetHIBase();
	if(HeroInfoBackStored==0){
		Copy((Byte *)hp,(Byte *)HeroInfoBack,sizeof(HeroInfoBack));
		for(i=0;i<HERNUM;i++){
//asm int 3
			PicsBack[i].HPSLoaded=LoadPCX(hp[i].HPSName);
			PicsBack[i].HPLLoaded=LoadPCX(hp[i].HPLName);
		}
		HeroInfoBackStored=1;
	}else{
		Copy((Byte *)HeroInfoBack,(Byte *)hp,sizeof(HeroInfoBack));
	}
	sp=(_HeroSpec_ *)0x678420;
	for(i=0;i<HERNUM_0;i++){
		HSpecTable[i].Spec=sp[i].Spec;
		HSpecTable[i].Spec1=sp[i].Spec1;
		HSpecTable[i].Spec2=sp[i].Spec2;
		HSpecTable[i].Spec3=sp[i].Spec3;
		HSpecTable[i].Spec4=sp[i].Spec4;
		HSpecTable[i].Spec5=sp[i].Spec5;
		HSpecTable[i].Spec6=sp[i].Spec6;
	}
/*
	for(i=0;i<HERNUM;i++){
		HeroInfoBack[i].HPSName
	}
*/
	for(i=0;;i++){
		ind=HeroSpecWoG[i].Ind;
		if(ind==-1) break;
		wp=&HeroSpecWoG[i];
		wp->SpecPicInd[1]=ind;
//      HeroSpecWoG[i].HPSName[1]=hp[ind].HPSName;
//      HeroSpecWoG[i].HPLName[1]=hp[ind].HPLName;
// Имя пока закоментировано
//      HeroSpecWoG[i].Name[1]=hp[ind].Name;
//      HeroSpecWoG[i].HBios[1]=HBiosTable[i].HBios;
////    wp->HPSLoaded[0]=LoadPCX(wp->HPSName[0]);
////    wp->HPSLoaded[1]=LoadPCX(wp->HPSName[1]);
////    wp->HPLLoaded[0]=LoadPCX(wp->HPLName[0]);
////    wp->HPLLoaded[1]=LoadPCX(wp->HPLName[1]);
		Copy((Byte *)&HSpecTable[ind],(Byte *)&wp->Spec[1],sizeof(_HeroSpec_));
		if(WoG)j=0; else j=1;
////    if((StrCmp(hp[ind].HPSName,wp->HPSName[0]))||
////       (StrCmp(hp[ind].HPSName,wp->HPSName[1]))){
////         tp=wp->HPSName[j]; tp1=tp; hp[ind].HPSName=tp1;
////    }
////    if((StrCmp(hp[ind].HPLName,wp->HPLName[0]))||
////       (StrCmp(hp[ind].HPLName,wp->HPLName[1]))){
////         tp=wp->HPLName[j]; hp[ind].HPLName=tp;
////    }
// Имя пока закоментировано
//    hp[ind].Name=HeroSpecWoG[i].Name[j];
//    HBiosTable[ind].HBios=HeroSpecWoG[i].HBios[j];
		Copy((Byte *)&wp->Spec[j],(Byte *)&HSpecTable[ind],sizeof(_HeroSpec_));
	}
	if(WoG){
/*
		for(i=0;i<HERNUM;i++){
			if(HeroSpecCus[i].Used==0) continue;
			cp=&HeroSpecCus[i];
			if(cp->HPLName!=0){
				hp[i].HPLName=cp->HPLName;
				cp->HPLLoaded=LoadPCX(cp->HPLName);
			}
			if(cp->HPSName!=0){
				hp[i].HPSName=cp->HPSName;
				cp->HPSLoaded=LoadPCX(cp->HPSName);
			}
		}
*/
		RefreshHeroPic();
	}
	RETURNV
}

void MapSetup(void)
{
	int wog;
	__asm pusha
	__asm{
		mov  eax,BASE
		mov  eax,[eax]
		add  eax,0x1F86C
		mov  eax,[eax]
		mov  DDummy,eax
	}
	STARTNA(__LINE__, 0)
	if(DDummy == SOGMAPTYPE || PL_ApplyWoG>1) wog = PL_NewHero; else wog=0;
	if(LoadCustomTxt(1,0)==0) Exit();
	PrepareSpecWoG(wog);
	STOP
	OriginalCallPointer=Callers[16].forig;
	__asm popa;
	__asm call dword ptr [OriginalCallPointer]
}

/////////////////////////////////
/*
void test1(void)
{
	_HeroInfo_ *hp;
	_HeroSpec_ *sp;
	asm{ mov eax,0x67DCE8; mov eax,[eax] } hp=(_HeroInfo_ *)_EAX;
	hp[0].HPSName=hp[3].HPSName;
	hp[0].HPLName=hp[3].HPLName;
	hp[0].Name="Ivan";
//  asm{ mov eax,0x678420 } sp=(_HeroSpec_ *)_EAX;
	sp=HSpecTable; // UN44.DEF, UN32.DEF
	sp[0].Spec=sp[15].Spec;
	sp[0].Spec1=sp[15].Spec1;
	sp[0].Spec2=sp[15].Spec2;
	sp[0].Spec3=sp[15].Spec3;
	sp[0].Spec4=sp[15].Spec4;
	sp[0]._u1=sp[15]._u1;
	sp[0]._u2=sp[15]._u2;
	sp[0].SpShort=sp[15].SpShort;
	sp[0].SpFull=sp[15].SpFull;
	sp[0].SpDescr=sp[15].SpDescr;
}
*/
////////////////////////////
// специализации героя
_Hero_ *hp;
Dword  ebp_;
int    hn;

void _SpecPicture(Dword po)
{
	STARTNA(__LINE__, 0)
	int i,pi,ind,n;
	//if(WoG==0) RETURNV
// edx (hp) -> герой, которому рисуют специализацию
// hp->Number = номер настоящей спец.
// [ebp-10] =
// проверяем на место вызова. EDX = ук на стр. героя или его номер
	if(((Dword)hp)<HERNUM) n=(int)hp; else n=hp->Number;
	for(i=0;;i++){
		ind=HeroSpecWoG[i].Ind;
		if(ind==-1) break;
		if(n==ind){
			pi=HeroSpecWoG[i].SpecPicInd[0];
			__asm{
				mov  eax,pi;
				mov  ebx,po
				mov  [ebx+0x18],eax // номер специализации
			}
		}
	}
	ind=Check4NewSpecPic(n);
	if(ind!=0){
		--ind;
		__asm{
			mov  eax,ind;
			mov  ebx,po
			mov  [ebx+0x18],eax // номер картинки специализации
		}
	}
	RETURNV
}

void __stdcall SpecPicture(Dword po)
{
	__asm pusha
	_EDX(hp);
	_SpecPicture(po);
	OriginalCallPointer=Callers[12].forig;
	__asm popa
	__asm push po
	__asm call dword ptr [OriginalCallPointer]
}

Dword _SpecPictureS(Dword spec)
{
	STARTNA(__LINE__, 0)
	int i,ind;
	//if(WoG==0) RETURN(spec)
// spec = номер настоящей спец.
	ind=Check4NewSpecPic(spec);
	if(ind!=0){
		RETURN(ind-1)// номер картинки специализации
	}
	for(i=0;;i++){
		ind=HeroSpecWoG[i].Ind;
		if(ind==-1) break;
		if((int)spec==ind){
			RETURN(HeroSpecWoG[i].SpecPicInd[0])
		}
	}
	RETURN(spec)
}

void __stdcall SpecPictureS(Dword d11,Dword d10,Dword d9,Dword d8,
												 Dword d7,Dword d6,Dword spec,Dword d4,
												 Dword d3,Dword d2,Dword d1)
{
	__asm pusha
//  hp=(_Hero_ *)_EDX;
	spec=_SpecPictureS(spec);
	OriginalCallPointer=Callers[14].forig;
	__asm popa
	__asm{ 
		push d1
		push d2
		push d3
		push d4
		push spec
		push d6
		push d7
		push d8
		push d9
		push d10
		push d11
		call dword ptr [OriginalCallPointer]
	}
}
/*
void _SpecPictureC(Dword *po)
{
//  if(WoG==0) return;
	int i;
	int Num=po[10];
	asm{
		mov  eax,BASE
		mov  eax,[eax]
		add  eax,0x1F86C
		mov  eax,[eax]
	}
	if(_EAX!=SOGMAPTYPE) return;
	for(i=0;i<Num;i++){
//    po[11+i] = тип героя
		po[11+i]=0;
	}
}

void SpecPictureC(void)
{
	Dword *po=(Dword *)_EAX;
	OriginalCallPointer=Callers[15].forig;
	asm call dword ptr [OriginalCallPointer]
	asm pusha
	_SpecPictureC(po);
	asm popa
}
*/
void _SpecPictureC(char **pname,int *pnum,char ** /*hname*/,Byte **spl)
{
	STARTNA(__LINE__, 0)
	int i,ind,wog,cn;
	// проверка на WoG
	__asm{
		mov  eax,BASE
		mov  eax,[eax]
		add  eax,0x1F86C
		mov  eax,[eax]
		mov  DDummy,eax
	}
	if(DDummy!=SOGMAPTYPE) wog=0; else wog=1;
	cn=*pnum;
	*spl=PicsBack[cn].HPSLoaded;
	for(i=0;;i++){
		ind=HeroSpecWoG[i].Ind;
		if(ind==-1) break;
		if(*pnum==ind){
//      if(HeroSpecWoG[i].HPSLoaded[1]==0){
//        HeroSpecWoG[i].HPSLoaded[1]=*spl;
//      }
			if(wog){
				*pnum=HeroSpecWoG[i].SpecPicInd[0];
				*pname=HeroSpecWoG[i].Spec[0].SpShort;
// Имя пока закоментировано
//        *hname=HeroSpecWoG[i].Name[0];
////        if((StrCmp((char *)&((*spl)[4]),HeroSpecWoG[i].HPSName[0]))||
////           (StrCmp((char *)&((*spl)[4]),HeroSpecWoG[i].HPSName[1])))
////           *spl=HeroSpecWoG[i].HPSLoaded[0];
			}else{
				*pnum=HeroSpecWoG[i].SpecPicInd[1];
				*pname=HeroSpecWoG[i].Spec[1].SpShort;
// Имя пока закоментировано
//        *hname=HeroSpecWoG[i].Name[1];
////        if((StrCmp((char *)&((*spl)[4]),HeroSpecWoG[i].HPSName[0]))||
////           (StrCmp((char *)&((*spl)[4]),HeroSpecWoG[i].HPSName[1])))
////           *spl=HeroSpecWoG[i].HPSLoaded[1];
			}
			break;
		}
	}
	if(wog){
		if((HeroSpecCus[cn].Used)&&(HeroSpecCus[cn].HPSName!=0)){
			 *spl=HeroSpecCus[cn].HPSLoaded;
			 RETURNV
		}
	}
	RETURNV
}

void __stdcall SpecPictureC(Byte *SPicLoaded,char *Name,Dword d3,int SpNum,char *SpName,Dword d0)
{
	__asm pusha
	_SpecPictureC(&SpName,&SpNum,&Name,&SPicLoaded);
	OriginalCallPointer=Callers[15].forig;
	__asm popa
	__asm{
		push   d0
		push   SpName
		push   SpNum
		push   d3
		push   Name
		push   SPicLoaded
		call   dword ptr [OriginalCallPointer]
	}
}

char *_SpecText(char *po)
{
	STARTNA(__LINE__, 0)
	int i,ind;
	//if(WoG==0) RETURN(po)
// edx (hp) -> герой, которому рисуют специализацию
// hp->Number = номер настоящей спец.
// po-> на короткое описание специализации
	for(i=0;;i++){
		ind=HeroSpecWoG[i].Ind;
		if(ind==-1) break;
		if(hp->Number==ind){
			RETURN(HeroSpecWoG[i].Spec[0].SpShort)
		}
	}
	RETURN(po)
}

//void SpecText(char *buf,char *po)
//{
//	__asm pusha
//	_EDX(hp);
//	po=_SpecText(po);
//	OriginalCallPointer=Callers[13].forig;
//	__asm popa
//	__asm push po
//	__asm push buf
//	__asm call dword ptr [OriginalCallPointer]
//	__asm add  esp,+8
//}

static char *nhname;
static Byte *nhspic;
//static int  _nothing[100];
void _NextHeroChoosen(void)
{
	STARTNA(__LINE__, 0)
	int i,ind,wog;
	// проверка на WoG
	__asm{
		mov  eax,BASE
		mov  eax,[eax]
		add  eax,0x1F86C
		mov  eax,[eax]
		mov  DDummy,eax
	}
	if(DDummy!=SOGMAPTYPE) wog=0; else wog=1;
//!!!!!! hn - не номер героя, а номер картинки!!!!!!!!
	if(hn>=HERNUM) RETURNV
//  cn=hn;
// непонятно, почему это?
//  nhspic=PicsBack[cn].HPSLoaded;
	for(i=0;;i++){
		ind=HeroSpecWoG[i].Ind;
		if(ind==-1) break;
		if(hn==ind){
////      if(wog){
// Имя пока закоментировано
//        nhname=HeroSpecWoG[i].Name[0];
////        if((StrCmp((char *)&nhspic[4],HeroSpecWoG[i].HPSName[0]))||
////           (StrCmp((char *)&nhspic[4],HeroSpecWoG[i].HPSName[1])))
////           nhspic=HeroSpecWoG[i].HPSLoaded[0];
////      }else{
// Имя пока закоментировано
//        nhname=HeroSpecWoG[i].Name[1];
////        if((StrCmp((char *)&nhspic[4],HeroSpecWoG[i].HPSName[0]))||
////           (StrCmp((char *)&nhspic[4],HeroSpecWoG[i].HPSName[1])))
////           nhspic=HeroSpecWoG[i].HPSLoaded[1];
////      }
			break;
		}
	}
	if(wog){
		if((HeroSpecCus[hn].Used)&&(HeroSpecCus[hn].HPSName[0])){
			 nhspic=HeroSpecCus[hn].HPSLoaded;
			 RETURNV
		}
	}
	RETURNV
}

void NextHeroChoosen(void)
{
/*
	asm mov  nhname,eax
	asm mov  eax,[eax-9]
	asm mov  hn,eax
	asm mov  eax,[esi+0xC8]
	asm mov  nhspic,eax
	asm pusha
	_NextHeroChoosen();
	asm popa
	asm mov  eax,nhspic
	asm mov  ecx,nhname
*/
	__asm mov  hn,eax
	__asm mov  ecx,[esi+4*eax+0xC8]
	__asm mov  nhspic,ecx
	__asm mov  ecx,[ebp+0x10]
	__asm mov  nhname,ecx
	__asm pusha
	_NextHeroChoosen();
	__asm popa
	__asm mov  ecx,nhname
	__asm mov  [ebp+0x10],ecx
	__asm mov  ecx,nhspic
}

/////////////////////////////////////////
int CalcHideArea(_Hero_ *Hp,int Correct)
{
	STARTNA(__LINE__, 0)
	int l,s,t,r;
	s=Hp->SpPoints;
	l=Hp->PSkill[3]; if(l<2) l=2;
	if(s>(l*10)) t=l; else t=s/10;
	r=0;
	while(t>1){
		t/=2; ++r;
	}
	if(Correct){
		if(s>(l*10)) Hp->SpPoints-=(Word)r;
	}
	if(r<1) r=1;
	RETURN(r)
}

void __stdcall HeroCheck(int NewX,int NewY,int Level,int Owner,int Radius,int Flag)
// у паскалевой процедуры аргументы в обратном порядке
{
	int     hn;
	Dword   basepo,po;
	_Hero_ *Hp;
//  void pascal (*OrFun)(int,int,int,int,int,int);

	_ECX(basepo);
	_ESI(Hp);
//  OrFun=(void pascal (*)(int,int,int,int,int,int))Callers[4].forig;
//  OrFun(NewX,NewY,Level,Owner,Radius,Flag);
// для "ослепленного" героя выход здесь.
	STARTNA(__LINE__, 0)
	//if(WoG){
		if(DoesHeroHas(Hp->Number,CURSE_BLIND)!=-1) RETURNV
		if(DoesHeroHas(Hp->Number,CURSE_NSCUT)!=-1) Radius=2;
	//}
	po=Callers[4].forig;
	__asm{
		push   Flag
		push   Radius
		push   Owner
		push   Level
		push   NewY
		push   NewX
		mov    esi,Hp
		mov    ecx,basepo
		mov    eax,po
		call   eax
	}
	//if(WoG==0) RETURNV
//if(Hp->Number!=0) return;
	hn=Hp->Number;
	// герой двигается - поищем ERM
	HeroMove(Hp,NewX,NewY,Level);
	if(CheckForCreature(Hp,155)==0){ // Darkness Dragon
		if(HSpecTable[hn].Spec!=HSUPERSPEC) RETURNV
		if(HSpecTable[hn].Spec1!=HSPEC_DARKNESS) RETURNV
	}
	HideArea(NewX,NewY,Level,Owner,CalcHideArea(Hp,0));
	RETURNV
}
/*
static int CheckForCreature(_Hero_ *hp,int type)
{
	for(int i=0;i<7;i++){
		if(hp->Ct[i]==type) return 1; // черный дракон
	}
	return 0;
}
*/
void MakeDarkness(int Owner)
{
	STARTNA(__LINE__, 0)
	int    i;
	_Hero_ *hp,*chp;
	//if(WoG==0) RETURNV
	if(Owner==-1) RETURNV
	__asm{
		mov  eax,BASE
		mov  eax,[eax]
		add  eax,0x21620
		mov  hp,eax
	}
	for(i=0;i<HERNUM;i++){
		chp=&hp[i];
		if(chp->Owner!=Owner) continue;
		if(CheckForCreature(chp,155)==0){ // Darkness Dragon
			if(HSpecTable[i].Spec!=HSUPERSPEC) continue;
			if(HSpecTable[i].Spec1!=HSPEC_DARKNESS) continue;
		}  
		HideArea(chp->x,chp->y,chp->l,Owner,CalcHideArea(chp,1));
	}  
	RETURNV
}

// 3.58 artifact left
static char _Art_Pics[64][60]; // шаблоны
static char _Art_Ind[64];
static _IconList IconList;
_IconListItem IconListItem[64]; // число арт в рюкзаке макс.

static _ZPrintf_ FHA_tmp;
int ChooseArt(_Hero_ *hp,int Remove)
{
	STARTNA(__LINE__, 0)
	int i,j,art;
	IconList.Items=IconListItem;
	IconList.Caption=ITxt(222,0,&Strings);
	IconList.ItemCount=hp->OANum;
	if(IconList.ItemCount<0) IconList.ItemCount=0;
	if(IconList.ItemCount>63) IconList.ItemCount=63;
	IconList.OnlyOne=1;
	IconList.ShowCancel=1;
	if(hp->OANum==0){ // нет артифактов в рюкзаке
		Message(ITxt(223,0,&Strings),1);
		RETURN(-1)
	}
	for(i=0,j=0;(i<64)&&(j<IconList.ItemCount);i++){
		if(hp->OArt[i][0]==-1) continue; // нет в слоте артика
		_Art_Ind[j]=(char)i;
		if(hp->OArt[i][0]==1){ // scroll
			IconListItem[j].Description=GetSpellName(hp->OArt[i][1]);
			Zsprintf2(&FHA_tmp,"[LODDEF]H3sprite.lod;spellint.def;%i;0;0",(Dword)hp->OArt[i][1]+2,0);
		}else{
			IconListItem[j].Description=ArtTable[hp->OArt[i][0]].Name;
			Zsprintf2(&FHA_tmp,"[LODDEF]H3sprite.lod;Artifact.def;%i;0;0",(Dword)hp->OArt[i][0]+1,0);
		}
		IconListItem[j].Checked=0;
		StrCopy(_Art_Pics[j],59,FHA_tmp.Str);
		IconListItem[j].Path=_Art_Pics[j];
		j++;
	}
//  IconListItem[0].Checked=1;
	ChooseIconDlg(&IconList);
	art=-1;
	for(i=0;i<IconList.ItemCount;i++){
		if(IconListItem[i].Checked){
			j=_Art_Ind[i];
			art=hp->OArt[j][0];
			if(art==1){ // scroll
				art=hp->OArt[j][1]+1000;
			}
			if(Remove){
/*
				for(;i<IconList.ItemCount-1;i++){
					hp->OArt[i][0]=hp->OArt[i+1][0];
					hp->OArt[i][1]=hp->OArt[i+1][1];
				}
*/
				hp->OANum--;
				hp->OArt[j][0]=-1;
				hp->OArt[j][1]=-1;
			}
			break;
		}
	}
	RETURN(art)
}

// Рисует картинку - название специализации
//004E1F2F 8B15708B6900   mov    edx,[00698B70] -> текущий герой
//004E1F35 8B0D809C6700   mov    ecx,[00679C80] -> HeroSpec.txt
//004E1F3B 8B421A         mov    eax,[edx+1A]   = номер (спец) героя
//004E1F3E 8D0480         lea    eax,[eax+4*eax]
//004E1F41 8B54C11C       mov    edx,[ecx+8*eax+1C] -> текст спец. героя
//004E1F45 52             push   edx
//004E1F46 6828746900     push   00697428      -> буфер
//004E1F4B E89C262200     call   007045EC      -> скопировать в буфер
//004E1F50 83C408         add    esp,00000008
//004E1F53 8D45D8         lea    eax,[ebp-28]
//004E1F56 BE03000000     mov    esi,00000003
//004E1F5B8BCB           mov    ecx,ebx
//004E1F5D 50             push   eax
//004E1F5E 8975DC         mov    [ebp-24],esi
//004E1F61 C745E08B000000 mov    dword ptr [ebp-20],0000008B
//004E1F68 C745F028746900 mov    dword ptr [ebp-10],00697428
//004E1F6F E82CD41100     call   HEROES3.005FF3A0

////////////////////
int SaveHeroData(void)
{
	STARTNA(__LINE__, 0)
	int i;
	if(Saver("LCRS",4)) RETURN(1)
/// 24.11.01 3.52
	for(i=0;i<HERNUM;i++){
		HeroSpecCus[i].HPSLoaded=0;
		HeroSpecCus[i].HPLLoaded=0;
	}
///
	if(Saver(HeroSpecCus,sizeof(HeroSpecCus))) RETURN(1)
	RETURN(0)
}

int LoadHeroData(int /*ver*/)
{
	STARTNA(__LINE__, 0)
	char buf[4]; if(Loader(buf,4)) RETURN(1)
	if(buf[0]!='L'||buf[1]!='C'||buf[2]!='R'||buf[3]!='S')
			{MError("LoadCurse cannot start loading"); RETURN(1) }
	if(Loader(HeroSpecCus,sizeof(HeroSpecCus))) RETURN(1)
	RefreshHeroPic();
	RETURN(0)
}

static int COH_Num;
void _CarryOverHero(void)
{
	STARTNA(__LINE__, 0)
	CrossOverNPC(COH_Num);
//  CrossOverGodBonus(COH_Num);
	RETURNV
}
Dword CarryOverHero(void)
{
	__asm pusha
	__asm mov  COH_Num,ecx
	_CarryOverHero();
	__asm popa
	__asm lea eax,[esi+0x1F458];
	__asm mov DDummy,eax
	return DDummy;
}
/*
char *_FixBioBug(int HeroNum)
{
	char *ret=GetHeroStr(HeroNum)->Bibl.m.s;
	if(ret==0){// no bio set
		ret=HBiosTable[HeroNum].HBios;
	}
	return ret;
}

__declspec( naked ) void FixBioBug(void)
{
	__asm{
		pusha
		push    edx
		call    _FixBioBug
		pop     edx
		mov     DDummy,eax
		popa
		mov     ecx,DDummy
		ret
	}
}
*/

void ResetHeroData(void)
{

}