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
	local mess = Header.."\n\n"..Text.."\n"
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
	local town_type = CA(-1):T(?v)
	local cr_available = Garrison.GetCreatureAvailable(-1,town_type)
	
	-- Get Cr that player can recruit
	local res = Res.GetPlayerRes(-1)
	local town_garrison = Garrison.GetTownGarrison(-1) -- is modified, but it isn't reflected in game
	local cr_table, cost_table = Garrison.GetCreatureToRecruit(res,cr_available,town_garrison)
	
	-- Show Dialog
	local flag = Dialog(cr_table,cost_table)
	if(flag == false) then return; end
	
	-- Remove resources
	local player_res = Res.GetPlayerRes(-1)
	Res.SetPlayerRes(-1,Res.Subtract(player_res,cost_table))
	
	-- Recruit 
	Garrison.RecruitCreature(-1,cr_table)
	
	-- Redraw
	UN:R(4,-1)
end
