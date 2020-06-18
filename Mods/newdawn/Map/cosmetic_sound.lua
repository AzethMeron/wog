-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

-- variables dealing with pathing
local sn_dir = ModName.."\\Res\\snd\\"
local sn_relative_dir = "..\\Mods\\"
local GetRandFile = Lib.GetRandFileMods
local sn_shut_down = sn_dir.."ZQUIET.WAV"

------------------- NEW SOUNDS -------------------
--------------------------------------

-- true if enabled
-- false if disabled
CheckSound = function()
  return global[ModName].NewSn
end

-- true if enabled
-- false if disabled
CheckSoundCondition = function()
  if(Lib.CheckIfEnabledCosmetic() ~= true) then return false end
  if(CheckSound() ~= true) then return false end
  return true;
end

--------------------------------------

-- Sounds replaced - not shut down
SN.? = function()
	if(CheckSoundCondition() ~= true) then return; end
	local InternalSounds = {
		["newday.wav"] = "newd*",
		["newweek.wav"] = "neww*",
		["newmonth.wav"] = "newm*",
		["treasure.wav"] = "arti*",
		["pickup01.82M"] = "piqu*",
		["pickup02.82M"] = "piqu*",
		["pickup03.82M"] = "piqu*",
		["pickup04.82M"] = "piqu*",
		["pickup05.82M"] = "piqu*",
		["pickup06.82M"] = "piqu*",
		["pickup07.82M"] = "piqu*",
		["nwherolv.82m"] = "lvlu*",
		["buildtwm.wav"] = "btwm*",
		["killfade.82M"] = "fade*",
		["GoodMrle.wav"] = "gmrl*",
		["DIGSOUND.82M"] = "digg*"
	}
	local mask = InternalSounds[SN:S(?z)]
	if(mask == nil) then return; end
	local snd_path = GetRandFile(sn_dir..mask)
	if(snd_path == nil) then return; end
	SN:S(sn_relative_dir..snd_path)
end

-- Sounds to shut down
SN.? = function()
	if(CheckSoundCondition() ~= true) then return; end
	local InternalSounds = {
		["Chest.wav"] = true,
		["Mystery.wav"] = true,
		["UltimateArtifact.wav"] = true,
		["Military.wav"] = true,
		["Rogue.wav"] = true,
		
		["Gazebo.wav"] = true,
		["GetProtection.wav"] = true,
		["Expernce.wav"] = true,
		["Genie.wav"] = true,
		["Nomad.wav"] = true,
		
		["Store.wav"] = true,
		["Faerie.wav"] = true,
		["Temple.wav"] = true,
		["Cavehead.wav"] = true,
		["Lighthouse.wav"] = true,
		
		["Dragon.wav"] = true,
		["Quest.wav"] = true,
		["Graveyard.wav"] = true,
		["Morale.wav"] = true,
		["Luck.wav"] = true,
		
		["Danger.wav"] = true,
		["flagmine.wav"] = true,
		["Obelisk.wav"] = true
	}
	if( InternalSounds[SN:S(?z)] == true) then
		SN:S(sn_relative_dir..sn_shut_down)
	end
end

--------------------------------------



