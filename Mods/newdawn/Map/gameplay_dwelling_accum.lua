-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

CheckAccumIfFlag = function()
	if (UN:P(34,?v) == 1) then return true
	else return false end
end

CheckAccumIfFlagCondition = function()
	if (Lib.CheckIfEnabledGameplay() ~= true) then return false end
	if (CheckAccumIfFlag() ~= true) then return false end
	return true
end

OB.? = function()
	if(CheckAccumIfFlagCondition ~= true) then return; end
	local ob_type = OB(998):T(?v)
	if (ob_type == 17) or (ob_type == 20) then
		local dw_owner = DW(998):O(?v)
		if(dw_owner ~= -1) then return; end
		for slot = 0,3,1 do
			local cr_type, cr_num = DW(998):M(slot,?v,?v)
			if(cr_type ~= -1) then
				local cr_growth = MA:G(cr_type,?v)
				if(cr_num > cr_growth) then
					DW(998):M(slot,cr_type,cr_growth)
				end
			end
		end
	end
end