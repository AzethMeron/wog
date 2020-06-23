-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

-- globals
local gModName = global[ModName]

--------------------------------- DWELL HINT ---------------------------------

-- Check if dwelling hint
-- return true if enabled
-- return false if disabled
CheckHintDwelling = function()
	return gModName.DwHint
end

-- Advanced dwelling's hint
HD.? = function()
	-- Check if Cosmetic AND Mod enabled
	if (Lib.CheckIfEnabledCosmetic() ~= true) then return; end
	if (CheckHintDwelling() ~= true) then return; end
	
	-- Stop if customized hint
	local customized = HD:C(?v)
	if (customized==1) then return; end
	
	-- Get object type, stop if NOT creature dwelling
	local ob_type = OB(998):T(?v)
	if (ob_type ~= 17) and (ob_type ~= 20) then return; end
	
	-- Check ownership/alliance
	local owner = DW(998):O(?v)
	if(owner == -1) then return; end
	local local_player = Lib.GetLocalPlayer()
	if (Lib.AlignmentCheckTeam(local_player,2^owner) == false) then return; end
	
	-- Creating hint
	local init_hint = HD:M(?z)
	local init_hint = "{"..init_hint.."}"
	local rightclick_hint = "\n"
	local mouseover_hint = ": "
	local img_counter = 0 -- for Hint Image
	for i = 0,3,1 do
		local cr_type, cr_num = DW(998):M(i,?v,?v)
		local cr_name = "Error AdvancedDwellHint: you shouldnt be seeing this"
		if(cr_type ~= -1) then
			if(cr_num == 1) then
				-- singular
				cr_name = UN:N3(?z,cr_type,0)
			else
				-- plural
				cr_name = UN:N3(?z,cr_type,1)
			end
			rightclick_hint = (rightclick_hint.."\n"..cr_name.." "..cr_num)
			mouseover_hint = (mouseover_hint..cr_name.." "..cr_num.." ")
			Lib.AddHintImage(img_counter,21,cr_type)
			img_counter = img_counter + 1
		end
	end
	
	-- Rightclick or mouseover
	local rightclick = HD:T(?v)
	if (rightclick == 0) then 
		-- mouseover
		HD:M(init_hint..mouseover_hint)
	else
		-- rightclick
		HD:M(init_hint..rightclick_hint)
	end
end

