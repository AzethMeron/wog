// to be included ONLY in Curse.cpp !!!

/* FORMERLY HARDCODED DATA */
const short int o_AS_CGood[CURSETYPE_NUM][3]={ {12,0,0}, 
									 {5,1,3},{7,1,3},{9,50,200},{15,1,6},{16,1,6},
									 {17,1,2},{18,1,2},{19,1,2},{20,1,2},{21,100,500},
									 {64,1,4},{65,100,500}};
const short int o_AS_CBad[CURSETYPE_NUM][3]={ {48,0,0}, 
									 {1,0,0},{2,-1,-1},{3,0,0},{4,100,500},{6,1,3},
									 {8,0,0},{10,100,300},{22,0,0},{23,0,0},{24,0,0},
									 {25,0,0},{26,0,0},{27,0,0},{28,0,0},{29,0,0},
									 {30,0,0},{31,0,0},{32,0,0},{33,0,0},{34,0,0},
									 {35,0,0},{36,0,0},{37,0,0},{38,0,0},{39,0,0},
									 {40,0,0},{41,0,0},{42,0,0},{43,0,0},{44,0,0},
									 {45,0,0},{46,0,0},{47,0,0},{48,0,0},{49,0,0},
									 {50,0,0},{51,0,0},{52,0,0},{53,0,0},{54,0,0},
									 {55,0,0},{56,0,0},{57,0,0},{58,0,0},{59,0,0},
									 {60,0,0},{61,0,0},{62,0,0}};

// Old object entrance prohibited table
const int o_DHVC_Table[CURSE_BLOCKS][3]={
// curse_id, ob_type, ob_subtype 
{22,109,-1}, // Water Wheel
{23,17,-1},  // Dwelling
{23,20,-1},  // Dwelling
{23,216,-1}, // Dwelling
{23,217,-1}, // Dwelling
{23,218,-1}, // Dwelling
{24,4,-1},   // Arena
{25,61,-1},  // Axis
{26,32,-1},  // Magic Garden
{27,100,-1}, // Learning Stone
{28,41,-1},  // Libr
{29,23,-1},  // Marletto
{30,51,-1},  // Merc camp A+1
{31,47,-1},  // 
{32,104,-1}, // Univer
{33,107,-1}, //
{34,113,-1}, // Witch Hut
{35,103,-1}, // Cave
{36,101,-1}, // Chest
{37,55,-1},  // Mystic Garden
{38,79,-1},  // All Resources
{39,97,-1},  // 
{40,31,-1},  // 
{41,84,-1},  // Crypt
{42,35,-1},  // Fort on Hill
{43,88,-1},  // Shrine
{43,89,-1},  // Shrine
{43,90,-1},  // Shrine
{44,42,-1},  // Lighthous
{45,13,1},   // Maps
{46,13,2},   // Maps
{47,13,0},   // Maps
{48,7,-1},   // Market
{49,43,-1},  // Teleporter
{49,44,-1},  // Teleporter
{50,57,-1},  // Obelisk
{51,60,-1},  // 
{52,99,-1},  // Post
{53,63,0},   // Piramid
{54,62,-1},  // Prison
{55,58,-1},  // 
{56,80,-1},  // Charch
{57,81,-1},  // Scholar
{58,94,-1},  // Stable
{59,102,-1}, // L.Tree
{60,95,-1},  // Tavern
{61,49,-1},  // Well
{62,111,-1}, // WP
//...
{0,0,0} // there must be it, otherwise segfault
};

const char o_GC_Pics[CURSETYPE_NUM][64]={ // Old pictures
"DATA\\ZVS\\LIB1.RES\\No1.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse1.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse2.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse3.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse4.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse5.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse6.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse7.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse8.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse9.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse10.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse11.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse12.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse13.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse14.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse15.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse16.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse17.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse18.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse19.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse20.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse21.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse22.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse23.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse24.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse25.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse26.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse27.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse28.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse29.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse30.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse31.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse32.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse33.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse34.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse35.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse36.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse37.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse38.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse39.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse40.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse41.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse42.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse43.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse44.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse45.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse46.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse47.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse48.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse49.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse50.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse51.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse52.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse53.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse54.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse55.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse56.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse57.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse58.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse59.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse60.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse61.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse62.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse63.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse64.bmp",
"DATA\\ZVS\\LIB1.RES\\Curse65.bmp",
//".\\DATA\\ZVS\\LIB1.RES\\Curse66.bmp",
//".\\DATA\\ZVS\\LIB1.RES\\Curse67.bmp",
//".\\DATA\\ZVS\\LIB1.RES\\Curse68.bmp",
//".\\DATA\\ZVS\\LIB1.RES\\Curse69.bmp",
//".\\DATA\\ZVS\\LIB1.RES\\Curse70.bmp",

"",//must be there, otherwise segfault
};