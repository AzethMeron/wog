#ifndef WL_ADDITION_ERM_HPP
#define WL_ADDITION_ERM_HPP

/* by Jakub Grzana
	This file is not supposed to be included anywhere except erm.h. 
	Simple addition, to separate old and new code. Don't try to compile this file seperately
*/

// Interface to change number of vars.
#define VAR_COUNT_V (10100) // Number of v-vars
#define VAR_COUNT_LZ (20) // Local z-vars
#define WL_FUNC_COUNT (40000) // Should be fully incorporated now. 
// If you change this: remember to change local wog_options_num = 1200 in Lua: options lua. Otherwise your data will be scraped
#define WL_PL_WONUM (1000) // Regular WoG options
#define WL_PL_WONUM_SPEC (200) // Hidden WoG options value. Note that new options are treated exactly the same EXCEPT they can't be changed with UN:P.

/* There used to be PL_WONUM wog options in the game, to be used with UN:P receiver
 I've decided to use the same array for storing values by my source addition. 
 Array is automatically stored in savefile, sent to other player in multiplayer mode
 so it's convenient. 
*/ 

// List of used hidden options - primarly to not overlap
// Please use [WL_PL_WONUM]+(number) instead of indexing directly, like 1050
#define WL_Questlog_mode (PL_WoGOptions[0][WL_PL_WONUM + 0])
#define WL_Questlog_func (PL_WoGOptions[0][WL_PL_WONUM + 1])

// Pointers for ERM triggers - don't change
#define WL_POINTER_HE (WL_FUNC_COUNT+2000) // Hero triggers
#define WL_POINTER_HM (WL_POINTER_HE+HERNUM+1) // Hero movement trigger
#define WL_POINTER_HL (WL_POINTER_HM+HERNUM+1) // Hero gain level pre trigger
#define WL_POINTER_HP (WL_POINTER_HL+HERNUM+1) // Hero gain level post trigger

#endif