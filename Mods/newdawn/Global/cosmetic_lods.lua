
local lodpath = ModName.."/Res/lod/"
local Lib = require "Lib"

function global.events.EnterMap()
	if(Lib.CheckIfEnabledCosmetic() ~= true) then return false end
	local gModName = global[ModName]
	local switch = {
		[false] = function() return; end,
		["oldwog"] = function() LD:T(?v,11,lodpath.."OldWog.lod") end,
		["bright"] = function() LD:T(?v,11,lodpath.."Trees.lod") end,
		["move"] = function() LD:T(?v,11,lodpath.."TreeMove.lod") end
	}
	switch[gModName.Trees]()
end

function global.events.EnterMap()
	if(Lib.CheckIfEnabledCosmetic() ~= true) then return false end
	local gModName = global[ModName]
	if gModName.NewGrid then
		LD:T(?v, 11, lodpath.."Grid.lod")
	end
end
