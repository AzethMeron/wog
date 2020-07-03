-- by Jakub Grzana
-- Library for abstract datatype - Resource table - operations on it

-- Manipulating data functions are NOT modifying arguments. You must get output value

-- Function that operates on actual players: GetPlayerRes, SetPlayerRes, AddPlayerRes 
-- Manipulating Resource table data: Truncate, Subtract, Add, MultiplyByValue, Revert
-- Analyze Resource table data: ShowRes, IsSufficent
-- Utilities: MonsterCost

--[[ Resource table = {
	[0] = wood_num,
	[1] = mercury_num,
	[2] = ore_num,
	[3] = sulfur_num,
	[4] = crystal_num,
	[5] = gems_num,
	[6] = gold_num,
	[7] = mithril_num
}
]]

------------------------------------------------------------------------------------------------

-- player - Format E1
-- return Resource table
GetPlayerRes = function(player)
	local res = {}
	for i = 0,7,1 do
		res[i] = OW:R(player,i,?v)
	end
	return res
end

-- player - Format E1
-- res - resource table
-- manual redraw required
SetPlayerRes = function(player, res)
	for i = 0,7,1 do
		OW:R(player,i,res[i])
	end
end

-- player - Format E1
-- res - resource table
-- manual redraw required
AddPlayerRes = function(player, res)
	for i = 0,7,1 do
		OW:R(player,i,{res[i]})
	end
end

------------------------------------------------------------------------------------------------

-- return table with indexes 0..7, filled with zeros.
Truncate = function()
	local res = {}
	for i = 0,7,1 do
		res[i] = 0
	end
	return res
end

-- base, to_subtract - resource table
-- return base - to_subtract
Subtract = function(base, to_subtract)
	local res = {}
	for i = 0,7,1 do
		res[i] = base[i] - to_subtract[i]
	end
	return res
end

-- base, to_add - resource table
-- return base + to_add
Add = function(base, to_add)
	local res = {}
	for i = 0,7,1 do
		res[i] = base[i] + to_add[i]
	end
	return res
end

-- base - resource table
-- multiplier - number
-- return base * multiplier
MultiplyByValue = function(base, multiplier)
	local res = {}
	for i = 0,7,1 do
		res[i] = base[i] * multiplier
	end
	return res
end

-- base - resource table
-- return base * (-1)
Revert = function(base)
	return MultiplyByValue(base,-1)
end

------------------------------------------------------------------------------------------------

-- res - resource table
-- for debug purposes
ShowRes = function(res)
	for i = 0,7,1 do
		IF:L(UX:S(741+i,?z).." "..res[i])
	end
end

-- bigger, lesser - resource table
-- return true, if all parts of lesser <= bigger
-- if any part of lesser > bigger, then false
IsSufficent = function(bigger, lesser)
	for i = 0,7,1 do
		if(lesser[i] > bigger[i]) then
			return false 
		end
	end
	return true
end

------------------------------------------------------------------------------------------------

-- mon_type - Format C
-- get cost of monster - all resources
-- including mithril, despite MA:C doesn't support it. Always [7] = 0
MonsterCost = function(mon_type)
	local cost = {}
	for i = 0,6,1 do
		cost[i] = MA:C(mon_type,i,?v)
	end
	cost[7] = 0
	return cost
end

------------------------------------------------------------------------------------------------