-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

-- true if enabled
-- false if disabled
CheckAINeutral = function()
	if UN:P(35,?v) == 1 then return true
	else return false end
end

-- true if enabled
-- false if disabled
CheckAINeutralCondition = function()
	if(Lib.CheckIfEnabledGameplay() ~= true) then return false end
	if(CheckAINeutral() ~= true) then return false end
	return true;
end

-- "globals"
local corr_atk = 0
local corr_def = 0
local corr_mig = 0
local hero_num = -3

BA(0).? = function()
	if(CheckAINeutralCondition() ~= true) then return; end
	local is_theoretical = BA:A(?v)
	if(is_theoretical == 0) then return; end
	-- Enabled and theoretical - so only AI
	local hero_atk, hero_def = BA:H(0,?v), BA:H(1,?v)
	-- Exit if there is hero defender - so fight hero vs neutral stack OR town
	if(hero_def ~= -2) then return; end
	-- Exit if there is town controlled by any player
	local ob_type = OB(998):T(?v)
	if(ob_type == 98) then 
		local town_owner = CA(998):O(?v)
		if (town_owner ~= -1) then return; end
	end
	-- So, theoretical battle, no defender hero, no-town or neutral town
	-- Parameters
	local linear = 0.34
	local flat = 3
	-- Calculations
	hero_num = hero_atk
	local atk, def, mig, trash = HE(hero_num):F(?v,?v,?v,?v)
	corr_atk = atk*linear + flat
	corr_def = def*linear + flat
	corr_mig = mig*linear + flat
	HE(hero_num):F( {corr_atk}, {corr_def}, {corr_mig}, nil)	
end

BA(1).? = function()
	if(CheckAINeutralCondition() ~= true) then return; end
	if(hero_num == -3) then return; end
	
	local t_corr_atk = corr_atk * (-1)
	local t_corr_def = corr_def * (-1)
	local t_corr_mig = corr_mig * (-1)
	HE(hero_num):F( {t_corr_atk}, {t_corr_def}, {t_corr_mig} ,nil)	
	
	-- reset
	corr_atk = 0
	corr_def = 0
	corr_mig = 0
	hero_num = -3
end