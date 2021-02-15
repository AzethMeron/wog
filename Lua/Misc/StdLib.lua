
StdLib = {}

local internal = debug.getregistry()
local Lib = StdLib
local type = type
local DbgMess = debug.Message

----------- No globals from this point ------------

local _NOGLOBALS

---------------------------------------------------

-- typical user shouldn't use internal.smt, and there should be some error management for tools
-- this is why i've made these
Lib.Lod = {
	Unload = function(ind) 
	-- ind - index of LOD file to be unloaded
	-- return -1 if there was error
	-- 		   0 if everything went smoothly
		if type(ind) ~= "number" then DbgMess("Incorrect type of 'ind' argument: "..type(ind)) return -1 end
		if ind < 0 then DbgMess("Incorrect value of 'ind' argument: "..ind) return -1 end
		internal.UnloadCustomLOD(ind)
		return 0
	end,
	
	Load = function(location, name, mode)
	-- location - modifier for location, like in UN:J8
	-- name - name of LOD file
	-- mode - mode/kind
	-- return index of loaded lod, -1 if error occured
		if type(mode) ~= "number" then DbgMess("Incorrect type of 'mode' argument: "..type(mode)) return -1 end
		if type(location) ~= "number" then DbgMess("Incorrect type of 'location' argument: "..type(location)) return -1 end
		if type(name) ~= "string" then DbgMess("Incorrect type of 'name' argument: "..type(name)) return -1 end
		if mode ~= internal.LodKindTemp and mode ~= internal.LodKindPerm and mode ~= internal.LodKindStored then DbgMess("Incorrect value of 'mode': "..mode) return -1 end
		if location < 0 or location > 11 then DbgMess("Incorrect value of 'location': "..location) return -1 end 
		if Lib.Universal.DoesFileExist(name,location) == 0 then DbgMess("There is no LOD file named: "..name) return -1 end
		return internal.LoadCustomLOD(location,name,mode)
	end
}

Lib.Objects = {
	Calculate = function(t,st)
	-- t - type
	-- st - subtype
	-- return: number of objects of given type/subtype
		if type(t) ~= "number" then DbgMess("Incorrect type of 'type' argument: "..type(t)) return -1 end
		if type(st) ~= "number" then DbgMess("Incorrect type of 'subtype' argument: "..type(st)) return -1 end
		return internal.CalcObjects(t,st)
	end,
	
	Iterate = function(t,st,func,args)
	-- t - type
	-- st - subtype
	-- func - function(x,y,z,args) to be called in loop
	-- args - arguments passed as "args" to "func"
	-- return -1 if error occured
		if type(t) ~= "number" then DbgMess("Incorrect type of 'type' argument: "..type(t)) return -1 end
		if type(st) ~= "number" then DbgMess("Incorrect type of 'subtype' argument: "..type(st)) return -1 end
		if type(func) ~= "function" then DbgMess("Incorrect type of 'function' argument: "..type(func)) return -1 end
		local num = internal.CalcObjects(t,st)
		local x, y, z = -1, 0, 0
		for i = 1,num,1 do
			x,y,z = internal.FindNextObjects(t,st,-1,x,y,z)
			func(x,y,z,args)
		end
		return 0
	end,
	
	Find = function(t,st,ind)
	-- t - type
	-- st - subtype
	-- ind - number of object (1..num)
	-- return x,y,z value
	-- if error occured, x,y,z are filled with 'nil'
		if type(t) ~= "number" then DbgMess("Incorrect type of 'type' argument: "..type(t)) return end
		if type(st) ~= "number" then DbgMess("Incorrect type of 'subtype' argument: "..type(st)) return end
		if type(ind) ~= "number" then DbgMess("Incorrect type of 'index' argument: "..type(ind)) return end
		local num = internal.CalcObjects(t,st)
		if ind < 1 or ind > num then DbgMess("Incorrect value of 'index' argument: "..type(ind)) return end
		local x, y, z = -1, 0, 0
		for i = 1, num, 1 do
			x,y,z = internal.FindNextObjects(t,st,-1,x,y,z)
			if i == ind then
				return x,y,z
			end
		end
	end
}

Lib.Universal = {
	DoesFileExist = function(name, location)
	-- name - name of file
	-- location - modifier for location, like in UN:J8
	-- check if file exists
	-- 0 means error or doesnt exists
	-- 1 means file exists
		if type(location) ~= "number" then DbgMess("Incorrect type of 'location' argument: "..type(location)) return 0 end
		if type(name) ~= "string" then DbgMess("Incorrect type of 'name' argument: "..type(name)) return 0 end
		return internal.DoesFileExist(name,location)
	end
}
