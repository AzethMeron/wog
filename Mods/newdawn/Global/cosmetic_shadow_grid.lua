-- By GrayFace
local i4, i2, i1, u4, u2, u1, call = mem.i4, mem.i2, mem.i1, mem.u4, mem.u2, mem.u1, mem.call
local Lib = require "Lib"

CheckShadowGrid = function()
	local gModName = global[ModName]
	if(gModName == nil) then return nil end
	return gModName.SoftGridShadow
end

CheckShadowGridCondition = function()
	if(Lib.CheckIfEnabledCosmetic() ~= true) then return false end
	if(CheckShadowGrid() ~= true) then return false end
	return true
end

local shadow_code = "\85\137\229\81\139\85\8\83\86\87\137\207\137\125\252\139\71\36\41\208\139\85\16\57\194\126\5\137\194\137\85\16\139\71\40\139\77\12\139\93\20\41\200\57\195\126\2\137\195\133\210\116\126\133\219\116\122\186\231\57\0\0\139\117\24\139\70\44\15\175\69\32\3\70\48\139\117\28\1\240\139\119\44\15\175\241\139\77\8\141\52\78\139\79\48\1\206\133\219\126\78\139\77\16\137\93\12\133\201\137\195\137\247\126\39\137\77\8\128\59\0\116\15\102\139\15\102\193\233\2\33\209\102\247\217\102\1\15\139\77\8\67\131\199\2\73\137\77\8\117\223\139\77\16\139\125\24\139\95\36\139\125\252\1\216\139\95\44\139\125\12\1\222\79\137\125\12\117\184\95\94\91\137\236\93\194\28\0\195\141\64\0"

local shadow_proc = call(mem.VirtualAllocPtr, 0, 0, #shadow_code, 0x1000, 0x40)
mem.copy(shadow_proc, shadow_code, #shadow_code)

function global.events.EnterContext()
	local shadow_proc = CheckShadowGridCondition() and shadow_proc or 0x44E370
	i4[0x4937B6+1] = shadow_proc - 0x4937BB
	i4[0x493C67+1] = shadow_proc - 0x493C6C
end
