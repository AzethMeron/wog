-- By GrayFace

local Lib = require "Lib"

CheckMovinMon = function()
	local gModName = global[ModName]
	if(gModName == nil) then return nil end
	return gModName.MonStandAnim
end

CheckMovinMonCondition = function()
	if(Lib.CheckIfEnabledCosmetic() ~= true) then return false end
	if(CheckMovinMon() ~= true) then return false end
	return true
end

function global.events.EnterMap()
	local ptr = debug.getregistry().EnableMovingMonstersPtr
	mem.i4[ptr] = CheckMovinMonCondition() and 1 or 0
end
