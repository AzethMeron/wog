#ifndef WL_QuestLog_C_HPP
#define WL_QuestLog_C_HPP

/* by Jakub Grzana
	This file is not supposed to be included anywhere except WLA_ERM.hpp. 
	Simple addition, to separate old and new code. Don't try to compile this file seperately
*/

#define QWESTLOGNUM 1000
struct _QuestLog_{
	int  num; //0-free
	int  z;
	int  hero;
	int  owner;
}QuestLog[QWESTLOGNUM];

_QuestLog_ *FindQuest(int num,int hero,int owner)
{
	STARTNA(__LINE__, 0)
	int i;
	for(i=0;i<QWESTLOGNUM;i++){
		if(QuestLog[i].num!=num) continue;
		if(QuestLog[i].hero!=hero) continue;
		if(QuestLog[i].owner!=owner) continue;
		RETURN(&QuestLog[i])
	}
	RETURN(0)
}

_QuestLog_ *AddQuest(int num,int hero,int owner)
{
	STARTNA(__LINE__, 0)
	int i;
	for(i=0;i<QWESTLOGNUM;i++){
		if(QuestLog[i].num!=0) continue;
		QuestLog[i].num=num;
		QuestLog[i].hero=hero;
		QuestLog[i].owner=owner;
		RETURN(&QuestLog[i])
	}
	RETURN(0)
}

void DelQuest(_QuestLog_ *qlp)
{
	STARTNA(__LINE__, 0)
	qlp->num=0;
	qlp->hero=0;
	qlp->owner=0;
	qlp->z=0;
	RETURNV
}
_QuestLog_ *GetQuest(int hero,int owner,int *ind)
{
	STARTNA(__LINE__, 0)
	int i;
	for(i=*ind;i<QWESTLOGNUM;i++){
		if(QuestLog[i].num==0) continue;
		if((QuestLog[i].hero!=hero)&&(QuestLog[i].hero!=-2)) continue;
		if((QuestLog[i].owner!=owner)&&(QuestLog[i].owner!=-2)) continue;
		*ind=i+1;
		RETURN(&QuestLog[i])
	}
	RETURN(0)
}

int ERM_Qwest(char Cmd,int Num,_ToDo_* /*sp*/,Mes *Mp)
{
	STARTNA(__LINE__,&Mp->m.s[Mp->i])
	int num,hero,owner;
	int value, func_to_call;
	_QuestLog_ *qlp;
	switch(Cmd){
		case 'A': // Anum/hero/owner/zvar;
		{
			CHECK_ParamsNum(4);
			if(Apply(&num,4,Mp,0)) { MError("\"!!QW:A\"-cannot get or check num."); RETURN(0) }
			if(Apply(&hero,4,Mp,1)) { MError("\"!!QW:A\"-cannot get or check hero number."); RETURN(0) }
			if(Apply(&owner,4,Mp,2)) { MError("\"!!QW:A\"-cannot get or check owner."); RETURN(0) }
			if(num<1){ MError("\"!!QW:A\"-Qwest number is incorrect (>0)."); RETURN(0) }
			if((hero<-2)||(hero>=HERNUM)){ MError("\"!!QW:A\"-hero number is incorrect."); RETURN(0) }
			if(hero==-1){
				hero=ERM_HeroStr->Number;
				if((hero<0)||(hero>=HERNUM)){ MError("\"!!QW:A\"-no current hero."); RETURN(0) }
			}
			if((owner<-2)||(owner>7)){ MError("\"!!QW:A\"-owner is incorrect (<-2 or >7)."); RETURN(0) }
			if(owner==-1) owner=CurrentUser();
			qlp=FindQuest(num,hero,owner);
			if(qlp==0) qlp=AddQuest(num,hero,owner);
			if(qlp==0){ MError("\"!!QW:A\"-cannot add one more quest."); RETURN(0) }
			if (Mp->VarI[3].Type != 7 && GetVarVal(&Mp->VarI[3]) == -1) // StrMan::Apply treats 0 as "restore original", not -1
				Mp->VarI[3].IType = Mp->VarI[3].Type = Mp->VarI[3].Num = Mp->n[3] = 0;

			char * str = StrMan::GetStoredStr(qlp->z);
			StrMan::Apply(str, 0, qlp->z, Mp, 3);
			if(qlp->z==0) DelQuest(qlp);
			break;
		}
		case 'M':
		{
			CHECK_ParamsMin(1);
			if(Apply(&value,4,Mp,0)) { WL_MError2("cannot get or check value."); RETURN(0) }
			if(value < 0 || value > 2) { WL_EWrongParam2("mode",value); RETURN(0) }
			if(value == 2)
			{
				CHECK_ParamsNum(2);
				if(Apply(&func_to_call,4,Mp,1)) { WL_MError2("cannot get or check number of function to call."); RETURN(0) }
				if(func_to_call < 1 || func_to_call > WL_FUNC_COUNT) { WL_EWrongParam2("function",func_to_call); RETURN(0) }
				WL_Questlog_func = func_to_call;
			}
			WL_Questlog_mode = value;
			break;
		}
		default: WL_EWrongCommand(); RETURN(0)
	}
	RETURN(1)
}

static _Hero_ *MQL_hp;
static Dword   MQL_ecx;
static char MQL_MesBuf[30000];
int _MakeQuestLog(void)
{
	STARTNA(__LINE__, 0)
	
	// Mode 2 - calling function instead. Copied from ERM_Function
	int ind,hero,owner;
	hero=MQL_hp->Number;
	owner=MQL_hp->Owner;
	if(WL_Questlog_mode == 2)
	{
		int args[2] = { owner, hero };
		Call_Function(WL_Questlog_func,args,2);
		RETURN(0)
	}
	// Mode 0 and 1
	_QuestLog_ *qlp;
	ind=0; MQL_MesBuf[0]=0;
	if(WoGType){ StrCanc(MQL_MesBuf,30000,MQL_MesBuf,"{Папирус}\n\n"); }
	else{ StrCanc(MQL_MesBuf,30000,MQL_MesBuf,"{Papyrus}\n\n"); }
	while((qlp=GetQuest(hero,owner,&ind))!=0){
		if (qlp->z > 0)
			StrCanc(MQL_MesBuf,30000,MQL_MesBuf,ERM2String(StrMan::GetStoredStr(qlp->z),1,0));
		else
			StrCanc(MQL_MesBuf,30000,MQL_MesBuf,StrMan::GetStoredStr(qlp->z));
		StrCanc(MQL_MesBuf,30000,MQL_MesBuf,"\n-------------------------------------------\n");
	}
	if(ind!=0){ // что-то было
		// Standard reaction
		if(WL_Questlog_mode == 0)
		{
			if(WoGType){ StrCanc(MQL_MesBuf,30000,MQL_MesBuf,"\n{Хотите} {посмотреть} {QuestLog?}"); }
			else{ StrCanc(MQL_MesBuf,30000,MQL_MesBuf,"\n{Do you want} {to} {see} {the} {QuestLog?}"); }
			RETURN(Request0(MQL_MesBuf))
		}
		// Override old questlog
		if(WL_Questlog_mode == 1)
		{
			Message(MQL_MesBuf);
			RETURN(0)
		}
	}
	// Empty papyrus
	if(WL_Questlog_mode == 1) { RETURN(0) }
	RETURN(1)
}

void MakeQuestLog(void)
{
	__asm pusha
	_EAX(MQL_hp); 
	_ECX(MQL_ecx);
	if(_MakeQuestLog()){
		__asm{
			mov   ecx,MQL_ecx
			mov   eax,0x52E910
			call  eax
		}
	}
	__asm popa
}

#endif