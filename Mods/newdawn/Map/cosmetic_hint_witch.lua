-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

-- globals
local GMap = _G[ModName].map
local gModName = global[ModName]

--------------------------------- WITCH HUT WITCH ---------------------------------
-- this script uses vind=1, should be changed later

CheckHintWitch = function()
	if(gModName == nil) then return nil end
	return gModName.WhHint
end

-- Initialise "map" variable for Witch's Hut
PI.? = function()
	local vind = 1 
	Lib.ObjectIterate(113,-1,vind,function(x,y,z,arg) GMap[x][y][z].WH = 0; end,nil)
end

-- Set witch hut visited
OB(113).? = function()
	-- Check if Mod enabled, cosmetics doesnt need to be enabled for setting visited
	if (Lib.CheckIfEnabledMod() ~= true) then return; end
	local bit_colors = GMap[ERM.v[998]][ERM.v[999]][ERM.v[1000]].WH
	local player = OW:C(?v)
	GMap[ERM.v[998]][ERM.v[999]][ERM.v[1000]].WH = bit.Or(bit_colors,2^player)
end

-- Advanced witch's hut hint
HD.? = function()
	-- Check if Cosmetic AND Mod enabled
	if (Lib.CheckIfEnabledCosmetic() ~= true) then return; end
	if (CheckHintWitch() ~= true) then return; end
	
	-- Stop if customized hint
	local customized = HD:C(?v)
	if (customized==1) then return; end
	
	-- Get object type, stop if NOT witch hut
	local ob_type = OB(998):T(?v)
	if (ob_type ~= 113) then return; end
	
	-- Creating hint
	local witch_hut_name = "{"..UX:S(2616,?z).."}"
	local bit_colors = GMap[ERM.v[998]][ERM.v[999]][ERM.v[1000]].WH
	local local_player = Lib.GetLocalPlayer()
	if (Lib.AlignmentCheckTeam(local_player,bit_colors) == false) then return; end
	local skill = WH(998):S(?v)
	-- Rightclick or mouseover
	local rightclick = HD:T(?v)
	if (rightclick == 0) then 
		-- mouseover
		HD:M(witch_hut_name.." ("..UN:N(4,?z,skill)..")")
	else
		-- rightclick
		HD:M(witch_hut_name.."\n"..UN:N(4,?z,skill))
		local skill_sq = 3*(skill+1)
		Lib.AddHintImage(0,20,skill_sq)
	end
end
