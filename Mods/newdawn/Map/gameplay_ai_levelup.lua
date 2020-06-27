-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

-- http://h3.heroes.net.pl/umiejetnosci/umiejetnosci-podstawowe
 
CheckAiLevelUp = function()
	if( UN:P(37,?v) == 1 ) then return true
	else return false end
end

CheckAiLevelUpCondition = function()
	if (Lib.CheckIfEnabledGameplay() ~= true) then return false end
	if (CheckAiLevelUp() ~= true) then return false end
	return true
end

