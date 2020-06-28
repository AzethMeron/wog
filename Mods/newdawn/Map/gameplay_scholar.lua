-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

CheckScholar = function()
	if (UN:P(31,?v) == 1) then return true
	else return false end
end

CheckScholarCondition = function()
	if (Lib.CheckIfEnabledGameplay() ~= true) then return false end
	if (CheckScholar() ~= true) then return false end
	return true
end

-------------------------------------------------------------
-- flag store false/true
flag = true
secondary_skill = -1 -- -1 means no-existant
-- flag=true means no changes to original behaviour
-- flag=false means "remove skill in posttrigger
-------------------------------------------------------------

-- for unknown skill
-- message - string
-- skill_ss - secondary skill, format SS
-- return true (yes) or false (no)
ScholarDialog = function(message, skill_ss)
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

-------------------------------------------------------------

-- Scholar pretrigger - Human players only
OB(81).pre = function()
	-- Conditions
	if(CheckScholarCondition() ~= true) then return; end
	if(ERM.flags[1000] == false) then return; end
	if(SC(998):T(?v) ~= 1) then return; end
	-- -- Gather data
	local skill_offered = SC(998):S(?v)
	secondary_skill = skill_offered
	hero_data = Lib.GetHeroData(-1) -- hero_data is INITIAL data about hero, BEFORE visit to hut
	-- Check if hero has offered Secondary Skill
	if(hero_data.skills[skill_offered] == nil) then
		-- Hero has no SS offered by Scholar
		local message = LibStr.Scholar["AskForPermission"]
		-- Disable standard dialog
		OB(998):M(-1,1,0)
		-- Scholar
		flag = ScholarDialog(message,skill_offered)
	else
		-- Hero already has SS offered by Scholar
		flag = true
	end
end

-- Witch hut pretrigger - AI players only
OB(81).pre = function()
	-- Conditions
	if(CheckScholarCondition() ~= true) then return; end
	if(ERM.flags[1000] == true) then return; end
	if(SC(998):T(?v) ~= 1) then return; end
	-- Gather data
	local skill_offered = SC(998):S(?v)
	secondary_skill = skill_offered
	hero_data = Lib.GetHeroData(-1) -- hero_data is INITIAL data about hero, BEFORE visit to hut 
	-- Temporary
	flag = true -- temporary !!!
end

-------------------------------------------------------------

-- Common postrigger - both AI and human
OB(81).post = function()
	-- Conditions - check if enabled
	if(CheckScholarCondition() ~= true) then return; end
	if(secondary_skill == -1) then return; end
	-- Check which option chosen
	if(flag == true) then
		-- Skill chosen or already known
		-- do nothing
	else
		-- Skill NOT chosen
		local skill_offered = secondary_skill
		HE(-1):S(skill_offered,0)
	end
	secondary_skill = -1
end