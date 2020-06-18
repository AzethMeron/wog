-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

-- variables dealing with pathing
local mp3_dir = ModName.."\\Res\\mp3\\"
local mp3_relative_dir = "..\\Mods\\"
local GetRandFile = Lib.GetRandFileMods

------------------- NEW MUSIC -------------------

-- true if enabled
-- false if disabled
CheckMusic = function()
  return global[ModName].NewMp
end

-- true if enabled
-- false if disabled
CheckMusicCondition = function()
  if(Lib.CheckIfEnabledCosmetic() ~= true) then return false end
  if(CheckMusic() ~= true) then return false end
  return true;
end

SetMusic = function(index, mod_relative_path)
  if(mod_relative_path == nil) then Lib.PrintError("SetMusic","Settiing nil music at index: "..index.." Not actual error, more warning") return; end
  MP:S(index,mp3_relative_dir..mod_relative_path)
end

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
      [9] = "elemtown*.mpe"
    }
  for index, mask in pairs(masks_town) do
    local music = GetRandFile(mp3_dir..music_dir..mask)
    SetMusic(index,music)
  end
end

ShuffleMusicTerrain = function()
  local music_dir = "town\\"
  local masks_terrain = { 
      [10] = "dirt*.mp3",
      [11] = "sand*.mpe",
      [12] = "grass*.mp3",
      [13] = "snow*.mp3",
      [14] = "swamp*.mp3",
      [15] = "rough*.mpe",
      [16] = "underground*.mp3",
      [17] = "lava*.mp3",
      [18] = "water*.mp3"
    }
  for index, mask in pairs(masks_terrain) do
    local music = GetRandFile(mp3_dir..music_dir..mask)
    SetMusic(index,music)
  end
end

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

