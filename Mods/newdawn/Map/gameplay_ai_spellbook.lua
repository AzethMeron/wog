-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

CheckAiSpellbook = function()
	if( UN:P(36,?v) == 1 ) then return true
	else return false end
end

CheckAiSpellbookCondition = function()
	if (Lib.CheckIfEnabledGameplay() ~= true) then return false end
	if (CheckAiSpellbook() ~= true) then return false end
	return true
end

-- Can't be used in pre trigger
MainSpellbook = function()
	if(CheckAiSpellbookCondition() ~= true) then return; end
	-- Owner
	local owner_castle = CA(-1):O(?v)
	local owner_hero = HE(-1):O(?v)
	if(owner_castle ~= owner_hero) then return; end
	-- Exit if human player
	local current_player = OW:C(?v)
	if(OW:I(current_player,?v) == 0) then return; end
	-- Check if magic guild is built. Restore flag1
	local flag_backup = ERM.flags[1]
	CA(-1):B(3,0,1)
	local guild_is_built = ERM.flags[1]
	ERM.flags[1] = flag_backup
	if(guild_is_built ~= true) then return; end
	-- Manage resources
	local gold = OW:R(-1,6,?v)
	if(gold < 5499) then return; end
	local temp, is_equipped_spellbook = HE(-1):A2(1000,?v,?v)
	if(is_equipped_spellbook ~= 0) then return end
	-- Buy spellbook
	OW:R(-1,6,{-500})
	HE(-1):A(4,1000)
	HE(-1):M(15,1) -- give magic arrow
end

-- Doesn't work due to Lua/ERM error
OB(98).Post = function()
	MainSpellbook()
end
