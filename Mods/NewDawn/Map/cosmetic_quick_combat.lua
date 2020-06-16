-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

-------------------------- QUICK COMBAT SETTINGS --------------------------

if new then
	LastChosenQuickCombatSetting = {}
	for i = 0,7,1 do
		LastChosenQuickCombatSetting[i] = 1
	end
end

-- Check if conditions fulfilled for Quick Combat
QuickCombatCondition = function()
	-- Check if real battle
	if ( ERM.flags[1000] == false ) then return false; end
	-- Check if Cosmetic AND Mod enabled
	if (Lib.CheckIfEnabledCosmetic() ~= true) then return false; end
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
	Message(player_color.." "..Lib.GetLocalPlayer())
	if (player_color ~= Lib.GetLocalPlayer()) then return false end
	-- Conditions met
	return true;
end

-- player_hero will be -10 or -20 on remote defender side
-- player_hero will be -2 if no hero (on both sides)
QuickCombat = function(player_owner, player_hero)
	
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