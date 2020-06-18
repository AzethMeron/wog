-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

-- variables dealing with pathing
local mp3_dir = ModName.."\\Res\\mp3\\"
local mp3_relative_dir = "..\\Mods\\"
local GetRandFile = Lib.GetRandFileMods

SetMusic = function(index, mod_relative_path)
  if(mod_relative_path == nil) then Lib.PrintError("SetMusic","Settiing nil music at index: "..index.." Not actual error, more warning") return; end
  MP:S(index,mp3_relative_dir..mod_relative_path)
end

ShuffleMusicAi = function()
  
end

ShuffleMusicCombat = function()
  
end

ShuffleMusicTown = function()
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
end

ShuffleMusicTerrain = function()
  
end

ShuffleMusicBattleresults = function()
  
end

