-- by Jakub Grzana
-- uses vind !!!

-- Standarised, abstract way of dealing with garrison within:
--  * town
--  * hero
-- More will be programmed on-demand

-- Function that operates on actual objects: GetTownGarrison, GetHeroGarrison, SetTownGarrison, SetHeroGarrison
-- Manipulating garrison data: 
-- Analyze garrison data: 
-- Other: 

--[[ Garrison = {
	[slot] = { Type, Num }
}
slot - number
Type - number (-1 if none)
Num - number (0 if none)
]]

local Lib = require "Lib"

------------------------------------------------------------------------------------------------

-- Town: -1, indirect reference
-- Town might be table too, consisting of { x,y,z }
GetTownGarrison = function(town)
	local output = {}
	-- Manage {x,y,z} reference
	local vind = 1
	local x, y, z = nil, nil, nil
	local reference = town
	if(type(town) == "table") then
		reference = vind
		x, y, z = Lib.GetCoords(vind)
		Lib.SetCoords(vind,town[1],town[2],town[3])
	end
	
	-- Script itself
	local hero_in_castle = CA(reference):H(0,?v)
	if( hero_in_castle ~= -1 ) then
		-- Hero
		return GetHeroGarrison(hero_in_castle)
	else
		-- Town garrison
		for i = 0,6,1 do
			local cType, cNum = CA(reference):M2(i,?v,?v)
			output[i] = { Type = cType, Num = cNum }
		end
	end
	
	-- Restore ERM values
	if(type(town) == "table") then
		Lib.SetCoords(vind,x,y,z)
	end
	
	return output
end

-- Town: -1, indirect reference
-- Town might be table too, consisting of { x,y,z }
SetTownGarrison = function(town, garrison)
	-- Manage {x,y,z} reference
	local vind = 1
	local x, y, z = nil, nil, nil
	local reference = town
	if(type(town) == "table") then
		reference = vind
		x, y, z = Lib.GetCoords(vind)
		Lib.SetCoords(vind,town[1],town[2],town[3])
	end
	
	-- Script itself
	local hero_in_castle = CA(reference):H(0,?v)
	if( hero_in_castle ~= -1 ) then
		-- Hero
		SetHeroGarrison(hero_in_castle,garrison)
	else
		-- Town garrison
		for i = 0,6,1 do
			 CA(reference):M2(i, garrison[i].Type, garrison[i].Num)
		end
	end
	
	-- Restore ERM values
	if(type(town) == "table") then
		Lib.SetCoords(vind,x,y,z)
	end
end

-- hero: Format H or -1
GetHeroGarrison = function(hero)
	output = {}
	for i = 0,6,1 do 
		local cType, cNum = HE(hero):C(0,i,?v,?v)
		output[i] = { Type = cType, Num = cNum }
	end
	return output
end

-- hero: Format H or -1
SetHeroGarrison = function(hero, garrison)
	for i = 0,6,1 do 
		HE(hero):C(0,i, garrison[i].Type, garrison[i].Num)
	end
end

------------------------------------------------------------------------------------------------