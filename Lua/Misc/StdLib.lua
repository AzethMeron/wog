
StdLib = {}

local internal = debug.getregistry()
local Lib = StdLib
local type = type

----------- No globals from this point ------------

local _NOGLOBALS

---------------------------------------------------

-- typical user shouldn't use internal.smt, and there should be some error management for tools
-- this is why i've made these
Lib.Lod = {
	Unload = function(ind) 
		if type(ind) ~= "number" then return nil end
		if ind < 0 then return nil end
		internal.UnloadCustomLOD(ind)
		return 0
	end,
	
	Load = function(location, name, mode)
		if type(mode) ~= "number" or type(location) ~= "number" or type(name) ~= "string" then return nil end
		if mode ~= internal.LodKindTemp and mode ~= internal.LodKindPerm and mode ~= internal.LodKindStored then return nil end
		if location < 0 then return nil end 
		return internal.LoadCustomLOD(location,name,mode)
	end
}