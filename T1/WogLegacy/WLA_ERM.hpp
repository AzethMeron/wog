#ifndef WL_ADDITION_ERM_HPP
#define WL_ADDITION_ERM_HPP

/* by Jakub Grzana
	This file is not supposed to be included anywhere except erm.h. 
	Simple addition, to separate old and new code. Don't try to compile this file seperately
*/

#define VAR_COUNT_V (10100) // Number of v-vars
#define VAR_COUNT_LZ (20) // Local z-vars
#define WL_FUNC_COUNT (30000) // Not fully incorporated yet! Don't change

/* There used to be PL_WONUM wog options in the game, to be used with UN:P receiver
 I've decided to use the same array for storing values by my source addition. 
 Array is automatically stored in savefile, sent to other player in multiplayer mode
 so it's convenient. */
#define WL_PL_WONUM (1000) // UN:P vars
#define WL_PL_WONUM_SPEC (100) // Hidden WoG options value
// Please use [WL_PL_WONUM]+(number) instead of indexing directly, like 1050
// Note that new options are treated exactly the same EXCEPT they can't be changed with UN:P.

// List of used hidden options - primarly to not overlap
#define WL_Questlog_mode (WL_PL_WONUM+0)
#define WL_Questlog_func (WL_PL_WONUM+1)

#endif