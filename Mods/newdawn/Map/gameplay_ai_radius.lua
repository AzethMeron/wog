-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

CheckAiRadius = function()
	return Options.FasterAI
end

CheckAiRadiusCondition = function()
	if (Lib.CheckIfEnabledGameplay() ~= true) then return false end
	if (CheckAiRadius() ~= true) then return false end
	return true
end

PI.? = function()
	if (CheckAiRadiusCondition() ~= true) then return; end
	UN:J(4,8192)
end