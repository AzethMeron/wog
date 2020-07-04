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

-- return true/false
Dialog = function(cr_table, cost_table)

	SN:P("button.wav")

	local Header = LibStr.BuyAllCreatures.Header
	local Text = LibStr.BuyAllCreatures.Text
	local mess = Header.."\n"..Text.."\n"
	for i = 6,0,-1 do
		if (cr_table[i].Type ~= -1) then
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

DialogEmpty = function()
	SN:P("button.wav")
	local Header = LibStr.BuyAllCreatures.Header
	local NoCr = LibStr.BuyAllCreatures.NoCr
	IF:M(Header.."\n\n"..NoCr)
	return false
end

DialogManager = function(cr_table, cost_table)
	if( Garrison.CountStacks(cr_table) > 0) then
		return Dialog(cr_table, cost_table)
	else
		return DialogEmpty()
	end
end

-- num: 0..6, number in cr_available
-- town: -1 or indirect reference
-- return slot, -1 if not found
GetRealSlot = function(garrison, clicked_icon, town)
	local dwellings = {}
	local ind = 0
	for i = 0,6,1 do
		local backup_flag = ERM.flags[1]
		CA(town):B(3,30+i,1)
		local flag = ERM.flags[1]
		if(flag == true) then
			dwellings[ind] = garrison[i].Type
			ind = ind + 1
		end
		ERM.flags[1] = backup_flag
	end
	if(ind <= clicked_icon) then return -1 end
	return Garrison.FindCreature(garrison,dwellings[clicked_icon])
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
	if(ob_number < 164) or (ob_number > 170) then return end -- !!! for tests, should be replaced with 159 
	
	-- Gather data about town and player
	local town_type = CA(-1):T(?v)
	local cr_available = Garrison.GetCreatureAvailable(-1,town_type)
	local res = Res.GetPlayerRes(-1)
	local town_garrison = Garrison.GetTownGarrison(-1) -- is modified, but it isn't reflected in game

	-- Analyze Data
	local clicked_icon = ob_number - 164
	local tier_to_recruit = GetRealSlot(cr_available,clicked_icon,-1)
	if(tier_to_recruit == -1) then return; end
	local tmp = Garrison.Truncate()
	tmp[tier_to_recruit] = cr_available[tier_to_recruit]
	local cr_table, cost_table = Garrison.GetCreatureToRecruit(res,tmp,town_garrison)
	
	-- Show Dialog
	local flag = DialogManager(cr_table,cost_table)
	if(flag == false) then return; end
	
	-- Remove resources
	Res.SetPlayerRes(-1,Res.Subtract(res,cost_table))
	
	-- Recruit 
	Garrison.RecruitCreature(-1,cr_table)
	
	-- Redraw
	UN:R(4,-1)
end
