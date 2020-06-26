-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

CheckWitchHut = function()
	if (UN:P(31,?v) == 1) then return true
	else return false end
end

CheckWitchHutCondition = function()
	if (Lib.CheckIfEnabledGameplay() ~= true) then return false end
	if (CheckWitchHut() ~= true) then return false end
	return true
end

-- Witch hut trigger
OB(113).? = function()
	if(CheckWitchHutCondition() ~= true) then return; end
	local skill_offered = WH(998):S(?v)
end