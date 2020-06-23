-- By GrayFace

-- Shift all speeds, old Fast becomes new Normal, new Fast is even faster

local r4 = mem.r4
local Lib = require "Lib"

CheckFasterCombat = function()
	local gModName = global[ModName]
	if(gModName == nil) then return nil end
	return gModName.FasterCombat
end

CheckFasterCombatCondition = function()
	if(Lib.CheckIfEnabledCosmetic() ~= true) then return false end
	if(CheckFasterCombat() ~= true) then return false end
	return true
end

function global.events.EnterMap()
	mem.IgnoreProtection(true)

	-- r4[0x63CF7C] = 0.63  -- keep slow speed at 1 for graphics tests
	r4[0x63CF7C+4] = CheckFasterCombatCondition() and 0.4 or 0.63
	r4[0x63CF7C+8] = CheckFasterCombatCondition() and 0.25 or 0.4

	mem.IgnoreProtection(false)
end
