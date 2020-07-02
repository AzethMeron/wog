-- library script to manage resources
-- by Jakub Grzana

--[[ Resource table = {
	[0] = wood_num,
	[1] = mercury_num,
	[2] = ore_num,
	[3] = sulfur_num,
	[4] = crystal_num,
	[5] = gems_num,
	[6] = gold_num,
	[7] = mithril_num.
}
]]

-- return table, [resource_id Format R] = num
GetRes = function(player)
	local res = {}
	for i = 0,7,1 do
		res[i] = OW:R(player,i,?v)
	end
	return res
end

-- manual redraw required
SetRes = function(player, res)
	for i = 0,7,1 do
		OW:R(player,i,res[i])
	end
end

-- manual redraw required
AddRes = function(player, res)
	for i = 0,7,1 do
		OW:R(player,i,{res[i]})
	end
end

-- return table with indexes 0..7, filled with zeros.
Truncate = function()
	local res = {}
	for i = 0,7,1 do
		res[i] = 0
	end
	return res
end

-- for debug purposes
ShowRes = function(res)
	for i = 0,7,1 do
		IF:L(UX:S(741+i,?z).." "..res[i])
	end
end

-- return true, if all lesser res are indeed lesser 
IsSufficent = function(bigger, lesser)
	for i = 0,7,1 do
		if(lesser[i] > bigger[i]) then
			return false 
		end
	end
	return true
end

Subtract = function(base, to_subtract)
	local res = {}
	for i = 0,7,1 do
		res[i] = base[i] - to_subtract[i]
	end
	return res
end

Add = function(base, to_add)
	local res = {}
	for i = 0,7,1 do
		res[i] = base[i] + to_add[i]
	end
	return res
end

MultiplyByValue = function(base, multiplier)
	local res = {}
	for i = 0,7,1 do
		res[i] = base[i] * multiplier
	end
	return res
end

Revert = function(base)
	local res = {}
	for i = 0,7,1 do
		res[i] = base[i] * (-1)
	end
	return res
end

MonsterCost = function(mon_type)
	local cost = {}
	for i = 0,6,1 do
		cost[i] = MA:C(mon_type,i,?v)
	end
	cost[7] = 0
	return cost
end