-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

-- variables dealing with pathing
local mp3_dir = ModName.."/Res/mp3/"
local mp3_relative_dir = "../Mods/"
local GetRandFile = Lib.GetRandFileMods
-- buggy section (terrain) disabler
local enable_terrain_section = false

-- globals
local gModName = global[ModName]

------------------- NEW MUSIC -------------------

-- timer
local last_day_executed = ERM.c
TM(1,-1,1,255).first? = function()
  if(ERM.c <= last_day_executed) then return; end
  CallerTerrain()
  CallerAi()
  if(ERM.c % 3 ==2) then CallerTerrain() end
  last_day_executed = ERM.c
end

--------------------------------------

-- true if enabled
-- false if disabled
CheckMusic = function()
	if(gModName == nil) then return nil end
  return gModName.NewMp
end

-- true if enabled
-- false if disabled
CheckMusicCondition = function()
  if(Lib.CheckIfEnabledCosmetic() ~= true) then return false end
  if(CheckMusic() ~= true) then return false end
  return true;
end

CheckSuccess = function(percentage_chance)
  local random_number = math.random(1,100)
  return random_number <= percentage_chance
end

--------------------------------------

SetMusic = function(index, mod_relative_path)
  -- Ignore if nil - mod_relative_path might be nil, it is NOT a bug
  if(mod_relative_path == nil) then Lib.PrintError("SetMusic","Settiing nil music at index: "..index.." Not actual error, more warning") return; end
  -- Remove .mp3 and .MP3 extension
  local final_path = mp3_relative_dir..mod_relative_path
  final_path = final_path:sub(1,#final_path - 4)
  -- Set music
  --print(final_path)
  MP:S(index,final_path)
end

--------------------------------------

ShuffleMusicAi = function()
  local music_dir = "aitheme\\"
  local masks_ai = {
      [21] = "aitheme*.mp3",
      [22] = "aitheme*.mp3",
      [23] = "aitheme*.mp3" 
    }
  for index, mask in pairs(masks_ai) do
    local music = GetRandFile(mp3_dir..music_dir..mask)
    SetMusic(index,music)
  end
end

CallerAi = function()
  if (CheckMusicCondition() ~= true) then return; end
  if (CheckSuccess(25) ~= true) then return; end
  ShuffleMusicAi()
end

--------------------------------------

ShuffleMusicTown = function()
  local music_dir = "town\\"
  local masks_town = { 
      [1] = "cstletown*.mp3",
      [2] = "rampart*.mp3",
      [3] = "towertown*.mp3",
      [4] = "infernotown*.mp3",
      [5] = "necrotown*.mp3",
      [6] = "dungeon*.mp3",
      [7] = "stronghold*.mp3",
      [8] = "fortresstown*.mp3",
      [9] = "elemtown*.mp3"
    }
  for index, mask in pairs(masks_town) do
    local music = GetRandFile(mp3_dir..music_dir..mask)
    SetMusic(index,music)
  end
end

CallerTown = function()
  if (CheckMusicCondition() ~= true) then return; end
  if (CheckSuccess(25) ~= true) then return; end
  ShuffleMusicTown()
end

OB(98).first? = function()
  CallerTown()
end

--------------------------------------

ShuffleMusicTerrain = function()
  if(enable_terrain_section ~= true) then return; end
  local music_dir = "terrain\\"
  local masks_terrain = { 
      [10] = "dirt*.mp3",
      [11] = "sand*.mp3",
      [12] = "grass*.mp3",
      [13] = "snow*.mp3",
      [14] = "swamp*.mp3",
      [15] = "rough*.mp3",
      [16] = "underground*.mp3",
      [17] = "lava*.mp3",
      [18] = "water*.mp3"
    }
  for index, mask in pairs(masks_terrain) do
    local music = GetRandFile(mp3_dir..music_dir..mask)
    SetMusic(index,music)
  end
end

CallerTerrain = function()
  if (CheckMusicCondition() ~= true) then return; end
  if (CheckSuccess(25) ~= true) then return; end
  ShuffleMusicTerrain()
end

--------------------------------------

ShuffleMusicCombat = function()
  local music_dir = "combat\\"
  local masks_combat = { 
      [41] = "combat*.mp3",
      [42] = "combat*.mp3",
      [43] = "combat*.mp3",
      [44] = "combat*.mp3"
    }
  for index, mask in pairs(masks_combat) do
    local music = GetRandFile(mp3_dir..music_dir..mask)
    SetMusic(index,music)
  end
end

CallerCombat = function()
  if (CheckMusicCondition() ~= true) then return; end
  if (CheckSuccess(25) ~= true) then return; end
  ShuffleMusicCombat()
end

BA(52).first? = function()
  CallerCombat()
end

--------------------------------------

ShuffleMusicBattleresults = function()
  local music_dir = "battleresults\\"
  local masks_battleresults = {
      [50] = "losecastle*.mp3",
      [51] = "defendcastle*.mp3",
      [52] = "losecombat*.mp3",
      [53] = "retreatbattle*.mp3",
      [54] = "surrenderbattle*.mp3",
      [55] = "winbattle*.mp3"
    }
  for index, mask in pairs(masks_battleresults) do
    local music = GetRandFile(mp3_dir..music_dir..mask)
    SetMusic(index,music)
  end
end

CallerBattleresults = function()
  if (CheckMusicCondition() ~= true) then return; end
  if (CheckSuccess(25) ~= true) then return; end
  ShuffleMusicBattleresults()
end

BA(52).first? = function()
  CallerBattleresults()
end

--------------------------------------

InitMusic = function()
  if (CheckMusicCondition() ~= true) then return; end
  ShuffleMusicAi()
  ShuffleMusicCombat()
  ShuffleMusicTown()
  ShuffleMusicTerrain()
  ShuffleMusicBattleresults()
end

InitMusic()

--------------------------------------

