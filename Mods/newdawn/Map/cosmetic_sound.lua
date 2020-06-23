-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

-- variables dealing with pathing
--[[ -- Mods doesnt work cause path is too long (above 12 chars)
local sn_dir = ModName.."/Res/snd/"
local sn_relative_dir = "../Mods/"
local GetRandFile = Lib.GetRandFileMods
local sn_shut_down = sn_dir.."ZQUIET.WAV"
]]

local sn_dir = "s/"
local sn_relative_dir = ""
local GetRandFile = Lib.GetRandFileData
local sn_shut_down = "ZQUIET.WAV"

-- globals
local gNewSn = nil
if(global.GlobNd ~= nil) then  gNewSn = global.GlobND.NewSn end
-- Parts: Enabled, Replace, Object, ShutUp
-- all path are relative to Mods folder !!!

------------------- NEW SOUNDS -------------------

-- true if enabled
-- false if disabled
CheckSound = function()
	if(gNewSn == nil) then return nil end
	return gNewSn.Enabled
end

-- true if enabled
-- false if disabled
CheckSoundCondition = function()
  if(Lib.CheckIfEnabledCosmetic() ~= true) then return false end
  if(CheckSound() ~= true) then return false end
  return true;
end

SetSound = function(snd_path)
	if(snd_path == nil) then return; end
	-- print(sn_relative_dir..snd_path) -- TEMPORARY
	SN:S(sn_relative_dir..snd_path)
end

PlaySound = function(snd_path)
	if(snd_path == nil) then return; end
	-- print(sn_relative_dir..snd_path) -- TEMPORARY
	SN:P(sn_relative_dir..snd_path)
end

--------------------------------------

-- Insert data - replace
-- ["Sound to be replaced"] = "filemask"
PI.? = function()
	if(CheckSoundCondition() ~= true) then return; end
	if(gNewSn.Replace == nil) then gNewSn.Replace = {} end
	
	local path = sn_dir
	
	gNewSn.Replace["newday.wav"] = path.."newd*"
	gNewSn.Replace["newweek.wav"] = path.."neww*"
	gNewSn.Replace["newmonth.wav"] = path.."newm*"
	gNewSn.Replace["treasure.wav"] = path.."arti*"
	
	gNewSn.Replace["pickup01.82M"] = path.."piqu*"
	gNewSn.Replace["pickup02.82M"] = path.."piqu*"
	gNewSn.Replace["pickup03.82M"] = path.."piqu*"
	gNewSn.Replace["pickup04.82M"] = path.."piqu*"
	
	gNewSn.Replace["pickup05.82M"] = path.."piqu*"
	gNewSn.Replace["pickup06.82M"] = path.."piqu*"
	gNewSn.Replace["pickup07.82M"] = path.."piqu*"
	gNewSn.Replace["nwherolv.82m"] = path.."lvlu*"
	
	gNewSn.Replace["buildtwm.wav"] = path.."btwm*"
	gNewSn.Replace["battle00.wav"] = path.."batl*"
	gNewSn.Replace["battle01.wav"] = path.."batl*"
	gNewSn.Replace["battle02.wav"] = path.."batl*"
	
	gNewSn.Replace["battle03.wav"] = path.."batl*"
	gNewSn.Replace["battle04.wav"] = path.."batl*"
	gNewSn.Replace["battle05.wav"] = path.."batl*"
	gNewSn.Replace["battle06.wav"] = path.."batl*"
	
	gNewSn.Replace["battle07.wav"] = path.."batl*"
	gNewSn.Replace["killfade.82M"] = path.."kfde*"
	gNewSn.Replace["DIGSOUND.82M"] = path.."dsnd*"
end

-- Insert data - shutup
-- ["Sound to be shut up"] = true
PI.? = function()
	if(CheckSoundCondition() ~= true) then return; end
	if(gNewSn.ShutUp == nil) then gNewSn.ShutUp = {} end
	
	gNewSn.ShutUp["Chest.wav"] = true
	gNewSn.ShutUp["Mystery.wav"] = true
	gNewSn.ShutUp["UltimateArtifact.wav"] = true
	gNewSn.ShutUp["Military.wav"] = true
	
	gNewSn.ShutUp["Rogue.wav"] = true
	gNewSn.ShutUp["Gazebo.wav"] = true
	gNewSn.ShutUp["GetProtection.wav"] = true
	gNewSn.ShutUp["Expernce.wav"] = true
	
	gNewSn.ShutUp["Genie.wav"] = true
	gNewSn.ShutUp["Nomad.wav"] = true
	gNewSn.ShutUp["Store.wav"] = true
	gNewSn.ShutUp["Faerie.wav"] = true
	
	gNewSn.ShutUp["Temple.wav"] = true
	gNewSn.ShutUp["Cavehead.wav"] = true
	gNewSn.ShutUp["Lighthouse.wav"] = true
	gNewSn.ShutUp["Dragon.wav"] = true
	
	gNewSn.ShutUp["Quest.wav"] = true
	gNewSn.ShutUp["Graveyard.wav"] = true
	gNewSn.ShutUp["Morale.wav"] = true
	gNewSn.ShutUp["Luck.wav"] = true
	
	gNewSn.ShutUp["Danger.wav"] = true
	gNewSn.ShutUp["flagmine.wav"] = true
	gNewSn.ShutUp["Obelisk.wav"] = true
end

-- Insert data - object
-- [Type][Subtype] = "filemask"
-- if subtype isn't mandatory, use -1
PI.? = function()
	if(CheckSoundCondition() ~= true) then return; end
	if(gNewSn.Object == nil) then gNewSn.Object = {} end
	
	local path = sn_dir
	
	-- Chest.wav
	gNewSn.Object[101] = { [-1] = path.."chest*" }
	gNewSn.Object[82] = { [-1] = path.."chest*" }
	
	-- Mystery.wav
	gNewSn.Object[6] = { [-1] = path.."myst*" }
	gNewSn.Object[97] = { [-1] = path.."myst*" }
	gNewSn.Object[7] = { [-1] = path.."myst*" }
	gNewSn.Object[2] = { [-1] = path.."myst*" }
	gNewSn.Object[53] = { [7] = path.."myst*" }
	gNewSn.Object[57] = { [-1] = path.."myst*" }
	gNewSn.Object[22] = { [0] = path.."myst*" }
	gNewSn.Object[63] = { [0] = path.."myst*" }
	gNewSn.Object[63] = { [10] = path.."myst*" }
	gNewSn.Object[63] = { [11] = path.."myst*" }
	gNewSn.Object[63] = { [12] = path.."myst*" }
	gNewSn.Object[63] = { [13] = path.."myst*" }
	
	-- Military.wav
	gNewSn.Object[17] = { [-1] = path.."milit*" }
	gNewSn.Object[20] = { [-1] = path.."milit*" }
	gNewSn.Object[78] = { [-1] = path.."milit*" }
	gNewSn.Object[107] = { [-1] = path.."milit*" }
	gNewSn.Object[219] = { [-1] = path.."milit*" }
	gNewSn.Object[33] = { [-1] = path.."milit*" }
	gNewSn.Object[35] = { [-1] = path.."milit*" }
	
	-- Rogue.wav
	gNewSn.Object[16] = { [-1] = path.."roqe*" }
	gNewSn.Object[62] = { [-1] = path.."roqe*" }
	
	-- Gazebo.wav
	
end

--------------------------------------

-- Replace
SN.? = function()
	if(CheckSoundCondition() ~= true) then return; end
	local sound_name = SN:S(?z)
	local filemask = gNewSn.Replace[sound_name]
	if(filemask == nil) then return; end
	SetSound( GetRandFile(filemask) )
end

-- ShutUp
SN.? = function()
	if(CheckSoundCondition() ~= true) then return; end
	local sound_name = SN:S(?z)
	if(gNewSn.ShutUp[sound_name] ~= true) then return; end
	SetSound(sn_shut_down)
end

-- Object
OB.? = function()
	if(CheckSoundCondition() ~= true) then return; end
	local ob_type = OB(998):T(?v)
	local ob_subtype = OB(998):U(?v)
	if(gNewSn.Object[ob_type] ~= nil) then
		if(gNewSn.Object[ob_type][ob_subtype] ~= nil) then
			PlaySound( GetRandFile(gNewSn.Object[ob_type][ob_subtype]))
		elseif(gNewSn.Object[ob_type][-1] ~= nil) then
			PlaySound(GetRandFile(gNewSn.Object[ob_type][-1]))
		end
	end
end