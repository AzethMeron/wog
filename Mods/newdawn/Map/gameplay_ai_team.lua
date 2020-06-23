-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

CheckAiTeam = function()
	if (UN:P(33,?v) == 1) then return true
	else return false end
end

CheckAiTeamCondition = function()
	if (Lib.CheckIfEnabledGameplay() ~= true) then return false end
	if (CheckAiTeam() ~= true) then return false end
	return true
end

-- While description says "Ai sends resources at the end of its turn" it is more like "at the beginning of every day take resources of previous allied computer"

if new then
	-- To make sure timer happens only once, and to skip on day 1
	LastDayExecute = {}
	for i = 0,7,1 do
		LastDayExecute[i] = 1
	end
end

FindPrev = function(player_current)
	local player_current_team = OW:T(player_current,?v)
	local player_current_num_increased = player_current + 8 -- 8..15, use %8
	player_current_num_increased = player_current_num_increased - 1
	while (player_current_num_increased ~= player_current) do
		local i_player = player_current_num_increased % 8
		local i_player_ai, i_player_killed = OW:I(i_player,?v,?v)
		if(i_player_killed == 0) then
			if(i_player_ai == 1) then
				local i_player_team = OW:T(i_player,?v)
				if(player_current_team ==  i_player_team) then return i_player end
			end
		end
		player_current_num_increased = player_current_num_increased - 1
	end
	return -1
end

AiTeam = function(player_current)
	local player_previous = FindPrev(player_current)
	if(player_previous == -1) then return; end
	for res = 0,7,1 do
		local player_current_res = OW:R(player_current,res,?v)
		local player_previous_res = OW:R(player_previous,res,?v)
		
		OW:R(player_current,res,{player_previous_res})
		OW:R(player_previous,res,0)
		
		--IF:L("************")
		--IF:L("PLayer "..player_current.." from player "..player_previous)
		--IF:L("Res "..res.." "..player_current_res.."+"..player_previous_res.."="..OW:R(player_current,res,?v))
	end
end

TM(1,-1,1,255).? = function()
	if(CheckAiTeamCondition() ~= true) then return end
	local player_current = OW:C(?v)
	local player_ai = OW:I(player_current,?v)
	if player_ai == 1 then
		if LastDayExecute[player_current] < ERM.c then
			AiTeam(player_current)
			LastDayExecute[player_current] = ERM.c
		end
	end
end