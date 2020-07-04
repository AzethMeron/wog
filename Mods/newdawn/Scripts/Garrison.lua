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
-- Utilities: GetCreatureAvailable, GetCreatureToRecruit, RecruitCreature

--[[ Garrison = {
	[slot] = { Type, Num }
}
slot - number
Type - number (-1 if none)
Num - number (0 if none)
]]

local Lib = require "Lib"
local StdMon = require "StdMonsters"
local Res = require "Res"

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

-- castle ID: parameter, CA(castle_id). Might be -1 or indirect reference. No support for x/y/z for now
-- town_type: Format T
-- return Garrison table. Structure: 
-- [tier] - creature tier, 0..6. 
-- .Num - number of creatures available. 
-- .Type - number, creature type, Format C
GetCreatureAvailable = function(castle_id, town_type)
	local cr_available = {}
	for i = 0,6,1 do
		local cr_basic_num, cr_upgraded_num = CA(castle_id):M1(i, ?v, ?v)
		if(cr_basic_num > 0) then
			cr_available[i] = {
				Num = cr_basic_num,
				Type = StdMon.Town[town_type][i].Basic,
			}
		elseif(cr_upgraded_num > 0) then
			cr_available[i] = {
				Num = cr_upgraded_num,
				Type = StdMon.Town[town_type][i].Upgraded,
			}
		else 
			cr_available[i] = {
				Type = -1,
				Num = 0
			}
		end
	end
	return cr_available
end

-- res: Resource datatype
-- cr_available, town_garrison_: Garrison datatype
-- return table of creatures that player can recruit. with regard of garrison slots AND resources. Garrison: cr_table[tier] = { Num, Type = Format C }
-- together with table of resources. Resource table: cost_table = { [Res] = num }
GetCreatureToRecruit = function(res, cr_available, town_garrison_)
	local cr_table = {}
	local cost_table = Res.Truncate()
	local town_garrison = Copy(town_garrison_)
	-- iterate through creatures
	for i = 6,0,-1 do
		if(cr_available[i].Type ~= -1) then
			local slot = FindSlotForCreature(town_garrison,cr_available[i].Type)
			if( slot ~= -1 ) then
				for i_num = cr_available[i].Num, 1, -1 do
					local cost_per_unit = Res.MonsterCost(cr_available[i].Type)
					local cost = Res.MultiplyByValue(cost_per_unit,i_num)
					if(Res.IsSufficent(res,cost)) then
						res = Res.Subtract(res,cost)
						cr_table[i] = {
							Type = cr_available[i].Type,
							Num = i_num
						}
						cost_table = Res.Add(cost_table, cost)
						AddCreature(town_garrison,cr_available[i].Type,i_num)
						break
					end
				end
			end
		end
		-- For the sake of keeping Garrison format
		if(cr_table[i] == nil) then
			cr_table[i] = {
				Type = -1,
				Num = 0
			}
		end
	end 
	return cr_table, cost_table
end

-- Remove creatures stored in cr_table from creatures available for hire
-- and place them in garrison/defender hero
-- Resources are NOT removed, and there is NO check for sufficent slots
-- town: -1 or indirect reference
-- cr_table: garrison datatype
RecruitCreature = function(town, cr_table)
	local town_garrison = GetTownGarrison(town)
	for i = 0,6,1 do
		if(cr_table[i].Type ~= -1) then
			local basic, upg = CA(town):M1(i,?v,?v)
			if(upg > 0) then
				local num = cr_table[i].Num * -1
				CA(town):M1(i,nil,{num})
			elseif(basic > 0) then
				local num = cr_table[i].Num * -1
				CA(town):M1(i,{num},nil)
			end
			AddCreature(town_garrison,cr_table[i].Type,cr_table[i].Num)
		end
	end
	SetTownGarrison(town,town_garrison)
end

------------------------------------------------------------------------------------------------