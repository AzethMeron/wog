-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"
-- Need to move bgp to Data/p cause path too long for BA:B

-------------------------- NEW BATTLEFIELD PICTURES --------------------------

-- Find & set .pcx for given object
-- nil if not found
-- string if found
ObjectPCX = function(x,y,l)
	local ob_type = OB(x,y,l):T(?v)
	local ob_subtype = OB(x,y,l):U(?v)
	local picture_path = Lib.GetRandFileMods(ModName.."/Res/bgp/t"..ob_type.."s"..ob_subtype.."p*.pcx")
	if(picture_path == nil) then
		picture_path = Lib.GetRandFileMods(ModName.."/Res/bgp/t"..ob_type.."s-1p*.pcx")
	end
	return picture_path;
end

-- Check if border
-- return true if there is border - shouldnt replace terrain picture
CheckBorder = function(x,y,l)
	if(TR(x,y,l):G(?v) ~= 0 ) then return false end
	local main_square = TR(x,y,l):T(?v, nil,nil,nil,nil,nil,nil,nil)
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

BA(52).first? = function()
	local x, y, l = BA:P(?v,?v,?v)
	local pic_obj = ObjectPCX(x,y,l)
	if(pic_obj ~= nil) then
		BA:B("../Mods/"..pic_obj)
	end
end

--[[
!!VRz1&y4=0/y5=0:S^dirt^; [Standard fields]
!!VRz1&y4=1/y5=0:S^sand^;
!!VRz1&y4=2/y5=0:S^grass^;
!!VRz1&y4=3/y5=0:S^snow^;
!!VRz1&y4=4/y5=0:S^swamp^;
!!VRz1&y4=5/y5=0:S^rough^;
!!VRz1&y4=6/y5=0:S^subt^;
!!VRz1&y4=7/y5=0:S^lava^;

!!VRz1&y5=046:S^magic^; [Special Fields]
!!VRz1&y5=226:S^fiery^;
!!VRz1&y5=231:S^rockl^;
!!VRz1&y5=229:S^air^;
!!VRz1&y5=228:S^lucid^;
!!VRz1&y5=227:S^holy^;
!!VRz1&y5=222:S^clover^;
!!VRz1&y5=224:S^evil^;
!!VRz1&y5=021:S^cursed^; [Now we have the name in z1]
--]]
