-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"
local StdMon = require "StdMonsters"

CheckBuyCr = function()
	return global[ModName].BuyAllCr
end

CheckBuyCrCondition = function()
	if(Lib.CheckIfEnabledCosmetic() ~= true) then return false end
	if(CheckBuyCr() ~= true) then return false; end
	return true
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
	local cr_available = {}
	for i = 0,6,1 do
		local cr_basic_num, cr_upgraded_num = CA(-1):M1(i, ?v, ?v)
		if(cr_basic_num > 0) then
			cr_available[i] = {
				Num = cr_basic_num,
				Type = StdMon.Town[town_type][i].Basic,
				CostPerUnit = {}
			}
		elseif(cr_upgraded_num > 0) then
			cr_available[i] = {
				Num = cr_upgraded_num,
				Type = StdMon.Town[town_type][i].Upgraded,
				CostPerUnit = {}
			}
		end
		
		if(cr_available[i] ~= nil) then for j = 0,6,1 do -- doesnt work with mithril
			cr_available[i].CostPerUnit[j] = MA:C(cr_available[i].Type,j,?v)
		end end
	end
	
	-- Gather data about player's resources
	local res = {}
	for i = 0,7,1 do
		res[i] = OW:R(-1,i,?v)
	end
	
	-- cr_available table structure:
	-- [tier] - creature tier, 0..6. Only the ones with Num > 0 are present! 
	-- .Num - number of creatures available. 
	-- .Type - number, creature type, Format C
	-- .CostPerUnit - table, cost of single unit in resources. [res_type] = amount_of_res
	
	
	
end
