#ifndef WLA_COMMON_HPP
#define WLA_COMMON_HPP


/* by Jakub Grzana
	This file is not supposed to be included anywhere except common.h 
	Simple addition, to separate old and new code. Don't try to compile this file seperately
*/

#include <cstdio>

// Error macros - use those instead of MError
#define WL_MError(str) { MError(str); }
#define WL_MError2(str) { MError2(str); }
#define WL_MError3(str,arg) { char loc_buf[4096]; sprintf(loc_buf,str,arg); MError2(loc_buf); }

#define WL_EWrongCommand() { WL_MError2("unknown command.") }
#define WL_EWrongSyntax() { WL_MError2("wrong syntax.") }
#define WL_EWrongParam(val) { WL_MError3("invalid parameter value: %d",val) }
#define WL_EWrongParam2(name,val) { char loc_buf[4096]; sprintf(loc_buf,"invalid parameter '%s' value: %d",name,val); MError2(loc_buf); }
#define WL_EWrongParamsNum(num,str,n) { char loc_buf[4096]; sprintf(loc_buf,"wrong number of parameters: %d\nExpected %s %d",num,str,n); MError2(loc_buf); }

#endif