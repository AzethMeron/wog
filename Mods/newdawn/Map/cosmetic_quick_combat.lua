-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

-- globals
local gModName = global[ModName]

-------------------------- QUICK COMBAT SETTINGS --------------------------
-- this script uses vind, should be changed later
-- might not restore mana properly after being attacked by AI as remote player
-- should be checked. daemon_n

if new then
	FightingHero = -2
	StoredMana = 0
	LastChosenQuickCombatSetting = {}
	for i = 0,7,1 do
		LastChosenQuickCombatSetting[i] = 1
	end
end

CheckQuickCombat = function()
	if(gModName == nil) then return nil end
	return gModName.QuickCombat
end

-- Check if conditions fulfilled for Quick Combat
QuickCombatCondition = function()
	-- Check if real battle
	if ( ERM.flags[1000] == false ) then return false; end
	-- Check if Cosmetic AND Mod enabled
	if (Lib.CheckIfEnabledCosmetic() ~= true) then return false; end
	if (CheckQuickCombat() ~= true) then return false; end
	-- Get heros' owners
	local atk_owner, def_owner = BA:O(?v,?v)
	-- Get attacker controller
	local atk_controller = OW:I(atk_owner,?v)
	-- Get defender controller
	local def_controller = 0
	if(def_owner == -1) then def_controller = 1 end 
	if(def_owner ~= -1) then def_controller = OW:I(def_owner,?v) end
	-- Exit if both owners are AI or Human
	if(atk_controller == def_controller) then return false; end
	if(atk_controller == 0) then 
		player_color = atk_owner 
	elseif(def_controller == 0) then
		player_color = def_owner 
	else
		Lib.PrintError("QuickCombatCondition","Weird error regarding controllers")
		return false;
	end
	-- Exit if remote player attacked by AI
	if (player_color ~= Lib.GetLocalPlayer()) then return false end
	-- Conditions met
	return true;
end

-- player_owner will contain color (Format E1) of the ONLY humam player in the battle
-- player_hero will be -10 or -20 on remote defender side
-- player_hero will be -2 if no hero (on both sides)
QuickCombat = function(player_owner, player_hero)
	local vind = 1 -- to store message output
	-- Get strings from Library - so i wont have to do it later
	local Header = LibStr.QuickCombat["Header"]
	local Manually = LibStr.QuickCombat["Manually"]
	local Quick = LibStr.QuickCombat["Quick"]
	local ConserveMana = LibStr.QuickCombat["ConserveMana"]
	-- Manage "conserve mana" option if no hero
	local last_chosen = LastChosenQuickCombatSetting[player_owner]
	if(player_hero==-2) then ConserveMana = 0; if(last_chosen == 4) then last_chosen = 1 end end
	-- Message
	IF:G( 1, vind, last_chosen, Header, Manually, Quick, ConserveMana, 0, 0, 0, 0, 0, 0, 0, 0, 0)
	local chosen = ERM.v[vind]
	-- Set globals
	FightingHero = player_hero
	StoredMana = 0
	-- Manage options
	if( bit.And(chosen, 2^0) > 0 ) then -- Manually
		BA:Q(0)
	elseif( bit.And(chosen, 2^1) > 0 ) then -- QuickCombat
		BA:Q(1)
	elseif( bit.And(chosen, 2^2) > 0 ) then -- ConserveMana
		StoredMana = HE(FightingHero):I(?v)
		HE(FightingHero):I(0)
		BA:Q(1)
	end
	-- Send data to remote player
	
end

-- Local side
-- On remote computer, we should use HE-10 and HE-20 instead of numbers, thus separation
BA(0).? = function()
	if(QuickCombatCondition() == true) then
		local atk_owner, def_owner = BA:O(?v,?v)
		local player_owner = -1
		local player_hero = -1
		if( OW:I(atk_owner,?v) == 0 ) then
			player_owner = atk_owner
			player_hero = BA:H(0,?v)
		elseif ( OW:I(def_owner,?v) == 0 ) then
			player_owner = def_owner
			player_hero = BA:H(1,?v)
		end
		QuickCombat(player_owner,player_hero)
	end
end

-- Remote side. 
-- On remote computer, we should use HE-10 and HE-20 instead of numbers, thus separation
BA(50).? = function()
	if(QuickCombatCondition() == true) then
		local atk_owner, def_owner = BA:O(?v,?v)
		local player_owner = -1
		local player_hero = -1
		if( OW:I(atk_owner,?v) == 0 ) then
			player_owner = atk_owner
			player_hero = -10
		elseif ( OW:I(def_owner,?v) == 0 ) then
			player_owner = def_owner
			player_hero = -20
			if(BA:H(1,?v) == -2) then player_hero = -2 end
		end
		QuickCombat(player_owner,player_hero)
	end
end

-- Return mana if hero fled
BG(1).? = function()
	if(QuickCombatCondition() == true) then
		if (BU:C(?v) == 0) then return end
		BA:Q(0)
		if(FightingHero == -2) then return; end
		if(StoredMana == 0) then return; end
		HE(FightingHero):I(StoredMana)
		FightingHero = -2
		StoredMana = 0
	end
end

-- Return mana after battle
-- Not sure at which side should it be executed
BA(53).? = function()
	if(QuickCombatCondition() == true) then
		BA:Q(0)
		if(FightingHero == -2) then return; end
		if(StoredMana == 0) then return; end
		HE(FightingHero):I(StoredMana)
		FightingHero = -2
		StoredMana = 0
	end
end