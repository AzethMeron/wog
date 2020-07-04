-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"
local StdMon = require "StdMonsters"
local Res = require "Res"
local Garrison = require "Garrison"

CheckKeyMacros = function()
	return global[ModName].Interface
end

CheckKeyMacrosCondition = function()
	if(Lib.CheckIfEnabledCosmetic() ~= true) then return false end
	if(CheckKeyMacros() ~= true) then return false; end
	return true
end

-- return true if garrison was changed
Ctrl = function(garrison,slot)
	if(garrison[slot].Type == -1) then return false end
	local free_slot = Garrison.FindCreature(garrison,-1)
	if(free_slot == -1) then return false; end
	if(garrison[slot].Num < 2) then return false end
	garrison[slot].Num = garrison[slot].Num - 1
	garrison[free_slot].Type = garrison[slot].Type
	garrison[free_slot].Num = 1
	return true
end

-- return true if garrison was changed
Alt = function(garrison,slot)
	return false
end

-- return true if garrison was changed
-- return false if garrison wasn't changed 
-- return string "unrecognised" if none of known keys are recognised
ManageClickType = function(garrison,slot)
	-- for easier expansion. [VK_ID] = func(garrison, slot)
	-- func must return true if garrison was changed, or false if wasn't
	local tab = {
		[17] = Ctrl,
		--[18] = Alt,
	}
	
	for vk, func in pairs(tab) do
		if UX:K(vk,?v) == 1 then
			return tab[vk](garrison,slot)
		end
	end
	return "unrecognised"
end

-- hero screen
CM(2).? = function()
	if(CheckKeyMacrosCondition() ~= true) then return; end
	-- Check if current player
	if(Lib.GetLocalPlayer() ~= OW:C(?v)) then return; end
	-- Exit if wrong click_type
	local click_type = CM:S(?v)
	if(click_type ~= 12) then return; end
	-- Check if rightclick on castle icon
	local ob_number = CM:I(?v)
	if(ob_number < 68) or (ob_number > 74) then return; end
	
	local slot = ob_number - 68
	local garrison = Garrison.GetHeroGarrison(-1)
	local outcome = ManageClickType(garrison,slot)
	if(outcome == true) then 
		Garrison.SetHeroGarrison(-1,garrison)
		UN:R(3,-1)
	end
	if(outcome ~= "unrecognised") then 
		CM:R(0)
	end
end

-- town screen
CM(1).? = function()
	if(CheckKeyMacrosCondition() ~= true) then return; end
	-- Check if current player
	local LocalPlayer = Lib.GetLocalPlayer()
	if(LocalPlayer ~= OW:C(?v)) then return; end
	-- Exit if wrong click_type
	local click_type = CM:S(?v)
	if(click_type ~= 12) then return; end
	-- Check if rightclick on castle icon
	local ob_number = CM:I(?v)
	
	local outcome = "unrecognised"
	if(ob_number >= 115) and (ob_number <= 121) then 
		-- Town garrison 
		local town_owner = CA(-1):O(?v)
		if(town_owner ~= LocalPlayer) then return; end
		local garrison = Garrison.GetTownGarrison(-1)
		local slot = ob_number - 115
		outcome = ManageClickType(garrison,slot)
		if(outcome == true) then 
			Garrison.SetTownGarrison(-1,garrison)
			UN:R(4,-1)
		end
	elseif(ob_number >= 140) and (ob_number <= 146) then 
		local hero = CA(-1):H(1,?v)
		if(hero == -1) then return; end
		local hero_owner = HE(hero):O(?v)
		if(hero_owner ~= LocalPlayer) then return; end
		local garrison = Garrison.GetHeroGarrison(hero)
		local slot = ob_number - 140
		outcome = ManageClickType(garrison,slot)
		if(outcome == true) then 
			Garrison.SetHeroGarrison(hero,garrison)
			UN:R(4,-1)
		end
	else
		return;
	end

	if(outcome ~= "unrecognised") then 
		CM:R(0)
	end
end

CM(3).? = function()
	if(CheckKeyMacrosCondition() ~= true) then return; end
	-- Check if current player
	local LocalPlayer = Lib.GetLocalPlayer()
	if(LocalPlayer ~= OW:C(?v)) then return; end
	-- Exit if wrong click_type
	local click_type = CM:S(?v)
	if(click_type ~= 12) then return; end
	-- Check if rightclick on castle icon
	local ob_number = CM:I(?v)
	
	local hero = -2
	local slot = -1
	if(ob_number >= 13) and (ob_number <= 19) then 
		-- Left hero
		local l_hero = HE(-1):N(?v)
		if(LocalPlayer ~= HE(l_hero):O(?v)) then return; end
		hero = l_hero
		slot = ob_number - 13
	elseif(ob_number >= 20) and (ob_number <= 26) then 
		-- Right hero
		local x,y,z = Lib.GetCoords(998)
		local r_hero = HE(x,y,z):N(?v)
		if(LocalPlayer ~= HE(r_hero):O(?v)) then return; end
		hero = r_hero
		slot = ob_number - 20
	else
		return;
	end

	local outcome = "unrecognised"
	local garrison = Garrison.GetHeroGarrison(hero)
	
	outcome = ManageClickType(garrison,slot)
	if(outcome == true) then 
		Garrison.SetHeroGarrison(hero,garrison)
	end
	if(outcome ~= "unrecognised") then 
		CM:R(0)
	end
end
