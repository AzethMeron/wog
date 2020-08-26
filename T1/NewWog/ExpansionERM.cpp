#include "../common.h"
#include "../structs.h"
#include "../b1.h"
#include "../erm.h"
#include "ExpansionERM.h"
#include <cmath>

#define __FILENUM__ 33

_LegacyGenericData_ LegacyGenericData; 

void SendLegacyData(int* len, Byte** buf)
{
	STARTNA(__LINE__, 0)
	*len = sizeof(LegacyGenericData); // Accquire length of data - in bytes
	*buf = (Byte*)&LegacyGenericData; // Assign pointer to your data
	RETURNV;
}

void ReceiveLegacyData(int len, Byte* buf)
{
	STARTNA(__LINE__, 0)
	Byte* LegacyGenData = (Byte*) &LegacyGenericData; 
	for(int i = 0; i < len; ++i)
	{
		LegacyGenData[i] = buf[i];
	}
	RETURNV;
}

// return 0 if everything is fine, otherwise 1
int SaveLegacyData()
{
	STARTNA(__LINE__, 0)
	if(Saver(&LegacyGenericData,sizeof(LegacyGenericData))) RETURN(1);
	RETURN(0);
}

// return 0 if everything is fine, otherwise 1
int LoadLegacyData()
{
	STARTNA(__LINE__, 0)
	if(Loader(&LegacyGenericData,sizeof(LegacyGenericData))) RETURN(1);
	RETURN(0);
}