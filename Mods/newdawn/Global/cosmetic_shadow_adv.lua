-- By GrayFace

local Lib = require "Lib"

CheckShadowAdv = function()
	local gModName = global[ModName]
	if(gModName == nil) then return nil end
	return gModName.SoftShadow
end

CheckShadowAdvCondition = function()
	if(Lib.CheckIfEnabledCosmetic() ~= true) then return false end
	if(CheckShadowAdv() ~= true) then return false end
	return true
end

function global.events.EnterMap()
	local on = CheckShadowAdvCondition()
	mem.i1[0x47D5C5+1] = on and 0x23 or 0x09
	mem.i1[0x47D45E+1] = on and 0x1F or 0x05
end
