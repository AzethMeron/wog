-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

-- variables dealing with pathing
local bgp_dir = ModName.."/Res/bgp/"
local data_relative_dir = "../Mods/"
local GetRandFile = Lib.GetRandFileMods
-- Need to move bgp to Data/p cause path too long for BA:B ???

-- globals
local gModName = global[ModName]

-------------------------- NEW BATTLEFIELD PICTURES --------------------------

-- sequence
-- check if there is .pcx coresponding to object's type and subtype (t63s21p*.pcx)
-- if not found above, check if there is .pcx coresponding to object's type, with universal type (t63s-1p*.pcx)
-- if not found above, check if there is town, then get picture based on town subtype 
-- if not found above, check if there overlay and get picture based on overlay subtype
-- if not found above, check if there is border. If isn't, get picture based on terrain
-- Set picture, unless nil

-- Check if cosmetic battle pictures enabled
-- return true if enabled
-- return false if disabled
CheckBattlePictures = function()
	return gModName.BattlePic
end

-- Find & set .pcx for given object
-- nil if not found
-- string if found
ModuleObjectPCX = function(x,y,l,ob_type,ob_subtype)
	local picture_path = GetRandFile(bgp_dir.."t"..ob_type.."s"..ob_subtype.."p*.pcx")
	if(picture_path == nil) then
		picture_path = GetRandFile(bgp_dir.."t"..ob_type.."s-1p*.pcx")
	end
	return picture_path;
end

-- return val
-- nil means not found
-- otheriwise, string relative to /Mods/
ModuleCastlePCX = function(ob_subtype)
	local masks_for_towns = {
		[0] = "grass*.pcx", -- Castle
		[1] = "grass*.pcx", -- Rampart
		[2] = "snow*.pcx", -- Tower
		[3] = "lava*.pcx", -- Inferno
		[4] = "dirt*.pcx", -- Necropolis
		[5] = "subt*.pcx", -- Dungeon
		[6] = "rough*.pcx", -- Stronghold
		[7] = "swamp*.pcx", -- Fortress
		[8] = "grass*.pcx" -- Conflux
	}
	return GetRandFile(bgp_dir..masks_for_towns[ob_subtype])
end

ModuleOverlayPCX = function(overlay)
	local masks_for_overlay = {
		[21] = "cursed*.pcx",
		[46] = "magic*.pcx",
		[222] = "clover*.pcx",
		[224] = "evil*.pcx",
		[225] = "winds*.pcx",
		[226] = "fiery*.pcx",
		[227] = "holy*.pcx",
		[228] = "lucid*.pcx",
		[229] = "air*.pcx",
		[231] = "rockl*.pcx"
	}
	return GetRandFile(bgp_dir..masks_for_overlay[overlay])
end

ModuleTerrainPCX = function(battle_square_terrain)
	local masks_for_terrain = {
		[0] = "dirt*.pcx",
		[1] = "sand*.pcx",
		[2] = "grass*.pcx",
		[3] = "snow*.pcx",
		[4] = "swamp*.pcx",
		[5] = "rough*.pcx",
		[6] = "subt*.pcx",
		[7] = "lava*.pcx"
	}
	return GetRandFile(bgp_dir..masks_for_terrain[battle_square_terrain])
end

-- Check if border
-- return true if there is border - shouldnt replace terrain picture
ModuleCheckBorder = function(x,y,l, main_square)
	for ix = x-1,x+1,1 do
		for iy = y-1,y+1,1 do
			if ((ix==x) and (iy == y)) then
				local local_square = TR(ix,iy,l):T(?v, nil,nil,nil,nil,nil,nil,nil)
				if(main_square ~=  local_square) then return true end
			end
		end
	end
	return false
end

BA(52).? = function()
	-- Check for cosmetic enhancements
	if(Lib.CheckIfEnabledCosmetic() ~= true) then return; end
	if(CheckBattlePictures() ~= true) then return; end
	-- Check if real battle
	if ( ERM.flags[1000] == false ) then return false; end
	-- Get coords of battle
	local x, y, l = BA:P(?v,?v,?v)
	local ob_type = OB(x,y,l):T(?v)
	local ob_subtype = OB(x,y,l):U(?v)
	local battle_square_terrain = TR(x,y,l):T(?v, nil,nil,nil,nil,nil,nil,nil)
	local overlay = TR(x,y,l):G(?v)
	-- Check for pcx coresponding to visited object (if any), exit if found
	local pic_obj = ModuleObjectPCX(x,y,l,ob_type,ob_subtype)	
	if(pic_obj ~= nil) then BA:B(data_relative_dir..pic_obj) return; end
	-- 
	local bgp = nil
	if( ob_type == 98 ) then bgp = ModuleCastlePCX(ob_subtype) 
	elseif( overlay ~= 0 ) then bgp = ModuleOverlayPCX(overlay)
	elseif(ModuleCheckBorder(x,y,l,battle_square_terrain) == false) then bgp = ModuleTerrainPCX(battle_square_terrain)
	end
	if(bgp ~= nil) then --[[Message(data_relative_dir..bgp)]] BA:B(data_relative_dir..bgp) end
end
