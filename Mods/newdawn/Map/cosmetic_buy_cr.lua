-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"
local StdMon = require "StdMonsters"

CM(1).? = function()
	local click_type = CM:S(?v)
	if(click_type ~= 12) then return; end
	local ob_number = CM:I(?v)
	if(ob_number ~= 158) then return end -- !!! for tests, should be replaced with 159
	-- rightclick on castle icon
	
	local cr_basic = {}
	local cr_upgraded = {}
	for i = 0,6,1 do
		cr_basic[i], cr_upgraded[i] = CA(-1):M1(i, ?v, ?v)
	end
	
	local res_gold = OW:R(-1,6,?v)
	local res_mercury = OW:R(-1,1,?v)
	local res_sulfur = OW:R(-1,3,?v)
	local res_crystal = OW:R(-1,4,?v)
	local res_gems = OW:R(-1,5,?v) 
	
	
	
end