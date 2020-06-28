-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

CheckWitchHut = function()
	if (UN:P(31,?v) == 1) then return true
	else return false end
end

CheckWitchHutCondition = function()
	if (Lib.CheckIfEnabledGameplay() ~= true) then return false end
	if (CheckWitchHut() ~= true) then return false end
	return true
end

-------------------------------------------------------------
-- flag store false/true
flag = true
-- flag=true means no changes to original behaviour
-- flag=false means "remove skill in posttrigger
-------------------------------------------------------------

-- for unknown skill
-- message - string
-- skill_ss - secondary skill, format SS
-- return true (yes) or false (no)
WitchHutDialogUnknown = function(message, skill_ss)
	-- Backup ERM flag 1
	local f_temp = ERM.flags[1]
	
	-- Script goes here
	local skill_sq = Lib.GetSQfromSS(skill_ss)
	IF:Q(1,20,skill_sq,2,message)
	local r_flag = ERM.flags[1]
	
	-- Restore ERM flag 1
	ERM.flags[1] = f_temp
	return r_flag
end

-- for known skill
-- message - string
-- skill_ss - secondary skill, format SS
-- no return
WitchHutDialogKnown = function(message, skill_ss)
	-- Backup ERM flag 1
	local f_temp = ERM.flags[1]
	
	-- Script goes here
	local skill_sq = Lib.GetSQfromSS(skill_ss)
	IF:Q(1,20,skill_sq,1,message)
	
	-- Restore flag 1
	ERM.flags[1] = f_temp
end

-------------------------------------------------------------

-- Witch hut pretrigger - Human players only
OB(113).pre = function()
	-- Conditions
	if(CheckWitchHutCondition() ~= true) then return; end
	if(ERM.flags[1000] == false) then return; end
	-- -- Gather data
	local skill_offered = WH(998):S(?v)
	hero_data = Lib.GetHeroData(-1) -- hero_data is INITIAL data about hero, BEFORE visit to hut
	-- Disable standard dialog
	OB(998):M(-1,1,0)
	-- Check if hero has offered Secondary Skill
	if(hero_data.skills[skill_offered] == nil) then
		-- Hero has no SS offered by Witch Hut
		local message = LibStr.WitchHut["AskForPermission"]
		flag = WitchHutDialogUnknown(message,skill_offered)
	else
		-- Hero already has SS offered by Witch Hut
		local message = LibStr.WitchHut["AlreadyKnown"]
		WitchHutDialogKnown(message,skill_offered) 
		flag = true
	end
end

-- Witch hut pretrigger - AI players only
OB(113).pre = function()
	-- Conditions
	if(CheckWitchHutCondition() ~= true) then return; end
	if(ERM.flags[1000] == true) then return; end
	-- Gather data
	local skill_offered = WH(998):S(?v)
	hero_data = Lib.GetHeroData(-1) -- hero_data is INITIAL data about hero, BEFORE visit to hut 
	-- Temporary
	flag = true -- temporary !!!
end

-------------------------------------------------------------

-- Common postrigger - both AI and human
OB(113).post = function()
	-- Conditions - check if enabled
	if(CheckWitchHutCondition() ~= true) then return; end
	-- Check which option chosen
	if(flag == true) then
		-- Skill chosen or already known
		-- do nothing
	else
		-- Skill NOT chosen
		local skill_offered = WH(998):S(?v)
		HE(-1):S(skill_offered,0)
	end
end