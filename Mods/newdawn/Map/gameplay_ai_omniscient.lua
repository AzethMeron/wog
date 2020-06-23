-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

CheckAiOmniscient = function()
	if (UN:P(32,?v) == 1) then return true
	else return false end
end

CheckAiOmniscientCondition = function()
	if (Lib.CheckIfEnabledGameplay() ~= true) then return false end
	if (CheckAiOmniscient() ~= true) then return false end
	return true
end

CheckAlliesOf = function(player)
	local player_team = OW:T(player,?v)
	for i_player = 0,7,1 do
		local is_computer, is_killed = OW:I(i_player,?v,?v)
		if(is_killed == 0) then
			if(is_computer == 0) then -- no need to exclude player from iteration, it wouldnt pass this if
				local i_player_team = OW:T(i_player,?v)
				if(player_team == i_player_team) then return false end
			end
		end
	end
	return true
end

PI.? = function()
	-- Condition
	if (CheckAiOmniscientCondition() ~= true) then return; end
	-- Get parameters
	local size, level = UN:X(?v,?v)
	local radius = ((size*141)/100)/2
	local center = (size/2)
	-- Activate for all AI players
	for i = 0,7,1 do
		local is_computer, is_killed = OW:I(i,?v,?v)
		if(is_killed == 0) then 
			if(is_computer == 1) then
				if(CheckAlliesOf(i)) then
					-- alive, computer, no human players
					for l = 0,level,1 do
						UN:S(center,center,l,i,radius)
					end
				end
			end
		end
	end	
end