-- by Jakub Grzana

local Lib = require "Lib"
local LibStr = require "LibStr"

-- globals
local gModName = global[ModName]

--------------------------------- ART HINT ---------------------------------

-- Check if hint artifact
-- return true if enabled
-- return false if disabled
CheckHintArtifact = function()
	return gModName.ArtHint
end

-- Advanced artifact's hint
HD.? = function()
	-- Check if Cosmetic AND Mod enabled
	if (Lib.CheckIfEnabledCosmetic() ~= true) then return; end
	if (CheckHintArtifact() ~= true) then return; end
	
	-- Stop if customized hint
	local customized = HD:C(?v)
	if (customized==1) then return; end
	
	-- Get object type, stop if NOT artifact
	local ob_type = OB(998):T(?v)
	if (ob_type ~= 5) then return; end
	
	-- Creating hint
	local art_type = OB(998):U(?v)
	-- Rightclick or mouseover
	local rightclick = HD:T(?v)
	if (rightclick == 0) then 
		-- mouseover
		local art_name = UN:N0(?z,art_type)
		local final_hint = "{"..UX:S(721,?z).."}".." ("..art_name..")"
		HD:M(final_hint)
	else
		-- rightclick
		local final_hint = "{"..( UX:S(721,?z) ).."}"
		HD:M(final_hint)
		HD:P(8,art_type)
	end
end



