-- Library script, for many common/universal functions
-- by Jakub Grzana

---------------------------------------------------------------------------------------------
------------------------------- CHECK FOR ENABLED MODULES -----------------------------------

-- return true if enabled or tournament
-- return false if disabled
CheckIfEnabledMod = function()
	if(UN:P(20,?v) > 0) then
		return true
	else
		return false
	end
end

-- return true if both mod and Cosmetic enabled
-- otherwise false
CheckIfEnabledCosmetic = function()
	if(CheckIfEnabledMod() == false) then return false end
	if(UN:P(21,?v) > 0) then
		return true
	else
		return false
	end
end

-- return true if both mod and Gameplay enabled
-- otherwise false
CheckIfEnabledGameplay = function()
	if(CheckIfEnabledMod() == false) then return false end
	if(UN:P(22,?v) > 0) then
		return true
	else
		return false
	end
end

-- return true if both mod and Content enabled
-- otherwise false
CheckIfEnabledContent = function()
	if(CheckIfEnabledMod() == false) then return false end
	if(UN:P(23,?v) > 0) then
		return true
	else
		return false
	end
end

-- return true if both mod and Object Replacement enabled
-- otherwise false
CheckIfEnabledObjectReplacement = function()
	if(CheckIfEnabledMod() == false) then return false end
	if(UN:P(23,?v) > 0) then
		return true
	else
		return false
	end
end

---------------------------------------------------------------------------------------------

-- player_color - Format E1
-- bit_colors - Format E
AlignmentCheck = function (player_color, bit_colors)
	-- Input arguments error handling
	if(player_color == nil) then PrintError("AlignmentCheck","Nil player_color value"); return false; end
	if(bit_colors == nil) then PrintError("AlignmentCheck","Nil bit_colors value"); return false; end
	-- Function
	local exp_player_color = 2^player_color
	if (exp_player_color:And(bit_colors) > 0) then 
		return true
	end
	return false
end

-- player_color - Format E1
-- bit_colors - Format E
AlignmentCheckTeam = function (player_color, bit_colors)
	if (player_color == -1) then PrintError("AlignmentCheckTeam","Passed -1 (neutral) as player_color, which may lead to unexpected behaviour") end
	local player_team = OW:T(player_color,?v)
	for i = 0,7,1 do
		local alliance_of_i = OW:T(i,?v)
		if(player_team == alliance_of_i) then 
			if (AlignmentCheck(i, bit_colors)) then
				return true;
			end
		end
	end
	return false
end

-- return player sitting in front of this PC in multiplayer mode
-- in singleplayer mode, return current player
-- -1 means some really weird error
GetLocalPlayer = function()
	local current_player = OW:C(?v)
	if(OW:G(current_player,?v) ~= 0) then
		return current_player
	else
		for i = 0,7,1 do
			if ( OW:G(i,?v) == 1 ) then
				return i
			end
		end
		PrintError("GetLocalPlayer","Undocumented error, local_player equal -1")
		return -1 
	end
end



-- func_name, err_mess - both strings
PrintError = function(func_name,err_mess)
	if((func_name == nil) or (err_mess == nil)) then print("Error while creating error message. Literally") return; end
	print(ModName..", function "..func_name..": "..err_mess)
end 

-- index: 0..2
AddHintImage = function(index,img_type,img_subtype)
	-- Input arguments error handling
	if(index == nil) then PrintError("AddHintImage","Nil index value"); return; end
	if(img_type == nil) then PrintError("AddHintImage","Nil img_type value"); return; end
	if(img_subtype == nil) then PrintError("AddHintImage","Nil img_subtype value"); return; end
	-- Function itself (nil means d0)
	if(index == 0) then
		HD:P(img_type,img_subtype,nil,nil,nil,nil)
	elseif (index == 1) then
		HD:P(nil,nil,img_type,img_subtype,nil,nil)
	elseif (index == 2) then
		HD:P(nil,nil,nil,nil,img_type,img_subtype)
	else
		PrintError("AddHintImage","Incorrect index value: "..index);
	end
end

-- Wrapper for WoG path.find() function that allows to easily get random file matching given mask. 
-- Both input and output values are relative to /Mods/ directory
GetRandFileMods = function(filemask)
	-- Initialising temp values
	local i = 1 -- tables in Lua should start with index 1
	local filetab = {}
	-- Getting all matching files into array
	for file in path.find("Mods/"..filemask,false) do
		filetab[i] = file
		i = i+1
	end
	-- If found none, return nil
	if #filetab == 0 then
		return nil
	end
	-- Getting Mods/ dir
	local dir = nil
	for tmp in path.find("Mods",true) do
		dir = tmp
	end
	if dir == nil then
		return nil
	end
	-- Returning given string
	local output = filetab[math.random(#filetab)]
	output = string.gsub(output,dir.."\\","")
	output = string.gsub(output,"\\","/")
	return (output)
end

-- Wrapper for WoG path.find() function that allows to easily get random file matching given mask. 
-- Both input and output values are relative to /Data/ directory
GetRandFileData = function(filemask)
	-- Initialising temp values
	local i = 1 -- tables in Lua should start with index 1
	local filetab = {}
	-- Getting all matching files into array
	for file in path.find("Data/"..filemask,false) do
		filetab[i] = file
		i = i+1
	end
	-- If found none, return nil
	if #filetab == 0 then
		return nil
	end
	-- Getting Data/ dir
	local dir = nil
	for tmp in path.find("Data",true) do
		dir = tmp
	end
	if dir == nil then
		return nil
	end
	-- Returning given string
	local output = filetab[math.random(#filetab)]
	output = string.gsub(output,dir.."\\","")
	output = string.gsub(output,"\\","/")
	return (output)
end

-- Type - type of object
-- Subtype - subtype of object (-1 for any)
-- vind - index of v-var that will store coords
-- func - function to call, arguments: x, y, z, args
-- args - argument you can pass to function inside loop. Use as table to pass multiple arguments
ObjectIterate = function(Type, Subtype, vind, func, args)
	local NumberOfObjects = UN:U(Type,Subtype,?v)
	ERM.v[vind] = -1
	ERM.v[vind+1] = -1
	ERM.v[vind+2] = 0
	for i = 1,NumberOfObjects,1 do
		UN:U(Type,Subtype,-1,vind)
		func(ERM.v[vind], ERM.v[vind+1], ERM.v[vind+2], args)
	end
end