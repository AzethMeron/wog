-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"
local vind = 1

-------------- TOOL TABLES --------------
-- Table of functions, to easily get owner of all supported objects
-- [Type]
local GetOwner = {
	-- Dwellings
	[17] = function(x,y,z) return DW(x,y,z):O(?v) end,
	[20] = function(x,y,z) return DW(x,y,z):O(?v) end,
	-- Garrisons
	[33] = function(x,y,z) return GR(x,y,z):O(?v) end,
	[219] = function(x,y,z) return GR(x,y,z):O(?v) end,
	-- Mines
	[53] = function(x,y,z) return MN(x,y,z):O(?v,1) end,
	[229] = function(x,y,z) return MN(x,y,z):O(?v,1) end,
	-- Shipyard
	[87] = function(x,y,z) return SY(x,y,z):O(?v) end,
	-- Castle
	[98] = function(x,y,z) return CA(x,y,z):O(?v) end,
}

-- Table of supported objects
local Types = { 17, 20, 33, 219, 53, 229, 87, 98 } 
------------------------------------------

------------- SETUP, DEFAULT -------------
if new then
	_G[ModName].Domination = {}
	local Domination = _G[ModName].Domination
	Domination.Mode = 0
	Domination.PointsPlayers = {}
	Domination.PointsMax = 100
	Domination.PointsIncomeBase = 2
	Domination.PointsLossBase = 1
	Domination.StdDialog = 1
	local x,y,z = Lib.GetCoords(vind)
	local GMap = _G[ModName].map
	for ind,i in pairs(Types) do
		Lib.ObjectIterate(i,-1,vind,function(x,y,z) GMap[x][y][z].Domination = { Points = 0, Name = nil } end)
	end
	Lib.SetCoords(vind,x,y,z)
end

local Domination = _G[ModName].Domination
------------------------------------------