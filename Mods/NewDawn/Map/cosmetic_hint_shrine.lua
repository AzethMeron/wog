-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

--------------------------------- SHRINE HINT ---------------------------------
-- this script uses vind=1, should be changed later

-- Initialise "map" variable for Shrines
PI.? = function()
	local vind = 1 
	Lib.ObjectIterate(88,-1,vind,function(x,y,z,arg) _G[ModName].map[x][y][z]["SR"] = 0; end,nil)
	Lib.ObjectIterate(89,-1,vind,function(x,y,z,arg) _G[ModName].map[x][y][z]["SR"] = 0; end,nil)
	Lib.ObjectIterate(90,-1,vind,function(x,y,z,arg) _G[ModName].map[x][y][z]["SR"] = 0; end,nil)
end

local SetShrineVisited = function(x,y,l)
	-- Check if Cosmetic AND Mod enabled
	if (Lib.CheckIfEnabledCosmetic() ~= true) then return; end
	local bit_colors = _G[ModName].map[x][y][l]["SR"]
	local player = OW:C(?v)
	_G[ModName].map[x][y][l]["SR"] = bit.Or(bit_colors,2^player)
end

-- Set shrine of magic incantation visited
OB(88).? = function()
	SetShrineVisited(ERM.v[998],ERM.v[999],ERM.v[1000])
end

-- Set shrine of magic gesture visited
OB(89).? = function()
	SetShrineVisited(ERM.v[998],ERM.v[999],ERM.v[1000])
end

-- Set shrine of magic thought visited
OB(90).? = function()
	SetShrineVisited(ERM.v[998],ERM.v[999],ERM.v[1000])
end

-- Advanced shrine's hint
HD.? = function()
	-- Check if Cosmetic AND Mod enabled
	if (Lib.CheckIfEnabledCosmetic() ~= true) then return; end
	
	-- Stop if customized hint
	local customized = HD:C(?v)
	if (customized==1) then return; end
	
	-- Get object type, stop if NOT shrine
	local ob_type = OB(998):T(?v)
	if ((ob_type ~= 88) and (ob_type ~= 89) and (ob_type ~= 90)) then return; end
	
	-- Creating hint
	local shrine_name = "{"..UX:S(2591+ob_type-88,?z).."}"
	local spell = SR(998):S(?v)
	local expanded_hint = LibStr.ShrineHint[ob_type]
	-- Check if visited
	local bit_colors = _G[ModName].map[ERM.v[998]][ERM.v[999]][ERM.v[1000]]["SR"]
	local local_player = Lib.GetLocalPlayer()
	local rightclick = HD:T(?v)
	if (Lib.AlignmentCheckTeam(local_player,bit_colors) == false) then 
		-- Hint if shrine not visited
		HD:M(shrine_name.."\n"..expanded_hint)
	else
		-- Hint if shrine visited
		if (rightclick == 0) then 
			-- mouseover
			HD:M(shrine_name.." ("..UN:N(1,?z,spell)..")")
		else
			-- rightclick
			HD:M(shrine_name.."\n"..expanded_hint)
			Lib.AddHintImage(0,9,spell)
		end
	end
end

