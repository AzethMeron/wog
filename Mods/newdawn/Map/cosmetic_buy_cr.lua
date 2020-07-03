-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"
local StdMon = require "StdMonsters"
local Res = require "Res"
local Garrison = require "Garrison"

CheckBuyCr = function()
	return global[ModName].BuyAllCr
end

CheckBuyCrCondition = function()
	if(Lib.CheckIfEnabledCosmetic() ~= true) then return false end
	if(CheckBuyCr() ~= true) then return false; end
	return true
end

-- cr_available table structure:
	-- [tier] - creature tier, 0..6. Only the ones with Num > 0 are present! 
	-- .Num - number of creatures available. 
	-- .Type - number, creature type, Format C
	-- .CostPerUnit - table, cost of single unit in resources. [res_type] = amount_of_res
GetCrAvailable = function(castle_id)
	local town_type = CA(-1):T(?v)
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
		end
		
		if(cr_available[i] ~= nil) then 
			cr_available[i].CostPerUnit = Res.MonsterCost(cr_available[i].Type) 
		end
	end
	return cr_available
end


-- cr_table = { Num, Type = Format C }
-- cost_table = { [Res] = num }
GetCrToRecruit = function(player, cr_available, free_slots)
	local cr_table = {}
	local cost_table = Res.Truncate()
	-- Gather available resources
	local res = Res.GetPlayerRes(player)
	-- iterate through creatures
	for i = 6,0,-1 do
		if(#cr_table >= #free_slots) then break end
		if(cr_available[i] ~= nil) then
			for i_num = cr_available[i].Num, 1, -1 do 
				local cost = Res.MultiplyByValue(cr_available[i].CostPerUnit,i_num)
				if(Res.IsSufficent(res,cost)) then
					res = Res.Subtract(res,cost)
					cr_table[i] = {
						Type = cr_available[i].Type,
						Num = i_num
					}
					cost_table = Res.Add(cost_table, cost)
					break
				end
			end
		end
	end 
	return cr_table, cost_table
end

-- return true/false
Dialog = function(cr_table, cost_table)
	local Header = LibStr.BuyAllCreatures.Header
	local Text = LibStr.BuyAllCreatures.Text
	local mess = Header.."\n\n"..Text.."\n"
	for i = 6,0,-1 do
		if (cr_table[i] ~= nil) then
			if(cr_table[i].Num > 1) then
				mess = mess.."\n"..UN:N(3,?z,cr_table[i].Type,1).." "..cr_table[i].Num
			elseif(cr_table[i].Num == 0) then
				mess = mess.."\n"..UN:N(3,?z,cr_table[i].Type,0).." "..cr_table[i].Num
			else
				Lib.PrintError("BuyAllCreatures:Dialog","number of creatures equal 0 - while shouldnt be")
			end
		end
	end
	
	local ind = 2
	local ThreeResources = { 
		[1] = { Type = 6, Num = cost_table[6] }
	}
	for i = 0,7,1 do
		if(i == 6) then i=i+1 end
		if(cost_table[i]>0) then 
			ThreeResources[ind] = {
				Type = i,
				Num = cost_table[i]
			}
			ind = ind + 1
		end
		if(ind >= 3) then break end
	end
	
	local flag = ERM.flags[1]
	if(#ThreeResources == 1) then
		local res_1 = ThreeResources[1]
		IF:Q(1,res_1.Type,res_1.Num,2,mess)
	elseif(#ThreeResources == 2) then
		local res_1 = ThreeResources[1]
		local res_2 = ThreeResources[2]
		IF:Q(1,res_1.Type,res_1.Num,res_2.Type,res_2.Num,2,mess)
	elseif(#ThreeResources > 2 ) then
		local res_1 = ThreeResources[1]
		local res_2 = ThreeResources[2]
		local res_3 = ThreeResources[3]
		IF:Q(1,res_1.Type,res_1.Num,res_2.Type,res_2.Num,res_3.Type,res_3.Num,2,mess)
	end
	local r_flag = ERM.flags[1]
	ERM.flags[1] = flag
	return r_flag
end

FreeSlotsInGarrison = function(town)
	local free_slots = {}
	for i = 0,6,1 do
		local Type, Num = CA(town):M2(i,?v,?v)
		if(Type == -1) then
			free_slots[i] = true
		end
		IF:L(Type.." " ..Num)
	end
	return free_slots
end

RecruitCr = function(town, cr_table, free_slots)
	for i = 0,6,1 do
		if(cr_table[i] ~= nil) then
			local basic, upg = CA(town):M1(i,?v,?v)
			if(upg > 0) then
				local num = cr_table[i].Num * -1
				CA(town):M1(i,nil,{num})
			elseif(basic > 0) then
				local num = cr_table[i].Num * -1
				CA(town):M1(i,{num},nil)
			end
			for j=0,6,1 do
				if(free_slots[j] ~= nil) then
					CA(town):M2(j,cr_table[i].Type,cr_table[i].Num)
					free_slots[j] = nil
					break
				end
			end
		end
	end
end

CM(1).? = function()
	if(CheckBuyCrCondition() ~= true) then return; end
	-- Check if current player
	if(Lib.GetLocalPlayer() ~= OW:C(?v)) then return; end
	-- Exit if wrong click_type
	local click_type = CM:S(?v)
	if(click_type ~= 12) then return; end
	-- Check if rightclick on castle icon
	local ob_number = CM:I(?v)
	if(ob_number ~= 158) then return end -- !!! for tests, should be replaced with 159 
	
	-- Gather data about creatures in town
	local cr_available = GetCrAvailable(-1)
	
	-- Get Cr that player can recruit
	local free_slots = FreeSlotsInGarrison(-1)
	local cr_table, cost_table = GetCrToRecruit(-1,cr_available,free_slots)
	
	-- Show Dialog
	local flag = Dialog(cr_table,cost_table)
	if(flag == false) then return; end
	
	-- Remove resources
	local player_res = Res.GetPlayerRes(-1)
	Res.SetPlayerRes(-1,Res.Subtract(player_res,cost_table))
	
	-- Recruit
	RecruitCr(-1,cr_table,free_slots)
	
	-- Redraw
	UN:R(4,-1)
end
