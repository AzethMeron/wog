-- by Jakub Grzana
-- uses vind !!!

-- Standarised, abstract way of dealing with garrison within:
--  * town
--  * hero
-- More will be programmed on-demand

-- Manipulating data functions are modifying arguments! 

-- Function that operates on actual objects: GetTownGarrison, GetHeroGarrison, SetTownGarrison, SetHeroGarrison
-- Manipulating garrison data: Truncate, AddCreature, Move, RemoveCreature, RemoveCreatureFromSlot
-- Analyze garrison data: FindCreature, FindSlotForCreature, CountStacks, ShowGarrison
-- Utilities: 

--[[ Garrison = {
	[slot] = { Type, Num }
}
slot - number
Type - number (-1 if none)
Num - number (0 if none)
]]

local Lib = require "Lib"

------------------------------------------------------------------------------------------------

-- return garrison
-- Town: -1, indirect reference
-- Town might be table too, consisting of { x,y,z }
-- tested - only for town = -1
GetTownGarrison = function(town)
	local output = Truncate()
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
		output = GetHeroGarrison(hero_in_castle)
	else
		-- Town garrison
		for i = 0,6,1 do
			local cType, cNum = CA(reference):M2(i,?v,?v)
			output[i].Type = cType 
			output[i].Num = cNum 
		end
	end
	
	-- Restore ERM values
	if(type(town) == "table") then
		Lib.SetCoords(vind,x,y,z)
	end
	
	return output
end

-- no return (nil)
-- Town: -1, indirect reference
-- Town might be table too, consisting of { x,y,z }
-- tested - only for town = -1
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

-- return garrison
-- hero: Format H or -1
-- tested
GetHeroGarrison = function(hero)
	output = Truncate()
	for i = 0,6,1 do 
		local cType, cNum = HE(hero):C(0,i,?v,?v)
		output[i].Type = cType 
		output[i].Num = cNum 
	end
	return output
end

-- no return (nil)
-- hero: Format H or -1
-- tested
SetHeroGarrison = function(hero, garrison)
	for i = 0,6,1 do 
		HE(hero):C(0,i, garrison[i].Type, garrison[i].Num)
	end
end

------------------------------------------------------------------------------------------------

-- return: empty Garrison format
Truncate = function()
	output = {}
	for i = 0,6,1 do
		output[i] = { Type = -1, Num = 0}
	end
	return output
end

-- Modifies passed garrison!
-- Add creature to garrison datatype.
-- return true, if succeed
-- return false, if failed 
-- cr_type: Format C, type of creature
-- cr_num: number
-- tested
AddCreature = function(garrison, cr_type, cr_num)
	local slot = FindSlotForCreature(garrison,cr_type)
	if(slot == -1) then return false end
	local num = cr_num
	if(garrison[slot].Type ~= -1) then num = num + garrison[slot].Num end
	garrison[slot].Type = cr_type
	garrison[slot].Num = num
	return true
end

-- Create copy of given garrison. Lua passes table as reference!
-- tested
Copy = function(garrison)
	local output = {}
	for i = 0,6,1 do
		output[i] = { Type = garrison[i].Type, Num = garrison[i].Num }
	end
	return output
end

-- Modifies passed garrisons!
-- from, to - garrison datatypes
-- tested
Move = function(from, to)
	for i = 6,0,-1 do
		if(from[i].Type ~= -1) then
			local slot = FindSlotForCreature(to,from[i].Type)
			if(slot ~= -1) then
				local num = from[i].Num + to[slot].Num
				to[slot].Type = from[i].Type
				to[slot].Num = num
				RemoveCreatureFromSlot(from,i)
			end
		end
	end
end

-- Modifies passed garrison!
-- Remove ANY creature from garrison on given slot
-- slot - 0..6
-- tested
RemoveCreatureFromSlot = function(garrison, slot)
	garrison[slot].Type = -1
	garrison[slot].Num = 0
end

-- Modifies passed garrison!
-- Remove ANY stack of given type from garrison
-- cr_type - Format C
-- tested
RemoveCreature = function(garrison,cr_type)
	local slot = FindCreature(garrison,cr_type)
	while(slot ~= -1) do
		RemoveCreatureFromSlot(garrison,slot)
		slot = FindCreature(garrison,cr_type)
	end
end

------------------------------------------------------------------------------------------------

-- return slot number where creature of given type is stored
-- return -1 if not found
-- cr_type: Format C, type of creature. Use -1 to find empty slot
-- tested
FindCreature = function(garrison, cr_type)
	for i = 0,6,1 do
		if garrison[i].Type == cr_type then return i end
	end
	return -1
end

-- return slot suitable to store given creature
-- First empty slot OR slot occupied by creature of same type
-- return -1 if not found
-- cr_type: Format C, type of creature
-- tested
FindSlotForCreature = function(garrison, cr_type)
	local slot = FindCreature(garrison, cr_type)
	if(slot == -1) then slot = FindCreature(garrison, -1) end
	return slot
end

-- return: number of stacks present
-- tested
CountStacks = function(garrison)
	counter = 0
	for i = 0,6,1 do 
		if(garrison[i].Type ~= -1) then
			counter = counter + 1
		end
	end
	return counter
end

-- for debug. 
-- tested
ShowGarrison = function(garrison)
	for i = 0,6,1 do
		local mess = "Slot "..i.." "
		if(garrison[i].Type ~= -1) then
			mess = mess.."Name "..(UN:N(3,?z,garrison[i].Type,1)).." Num "..garrison[i].Num
		else
			mess = mess.."Type "..garrison[i].Type.." Num "..garrison[i].Num
		end
		IF:L(mess)
	end
end

------------------------------------------------------------------------------------------------