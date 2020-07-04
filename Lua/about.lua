
local _G = _G
local internal = debug.getregistry()

local print = print
local Message = Message
local type = type
local unpack = unpack
local error = error
local pairs = pairs
local next = next
local ipairs = ipairs
local tonumber = tonumber
local tostring = tostring
local tostring2 = tostring2
local rawget = rawget
local rawset = rawset
local getmetatable = getmetatable
local setmetatable = setmetatable
local setmetatableW = internal.setmetatableW
local getfenv = getfenv
local setfenv = setfenv
local pcall = pcall
local assert = assert
local min = math.min
local max = math.max
local format = string.format
local string_match = string.match
local string_lower = string.lower
local string_find = string.find
local string_sub = string.sub
local table_copy = table.copy
local table_concat = table.concat
local path_find = path.find
local path_FindFirst = path.FindFirst
local path_AddSlash = path.AddSlash
local io_SaveString = io.SaveString
local io_LoadString = io.LoadString
local ReadIniString = ReadIniString
local loadfile = loadfile
local loadstring = loadstring
local debug_getinfo = debug.getinfo

local i4 = mem.i4
local mem_copy = mem.copy
local LoadTextTable = LoadTextTable
local WogOptionsPtr = internal.WogOptionsPtr

local UN_P = ERM.UN.P
local ?v = ?v

----------- No globals from this point ------------

local _NOGLOBALS

---------------------------------------------------

local AboutMods = {}
internal.AboutMods = AboutMods

-- AboutMods[modname].Cat[category][paragraph] = { Header, Text, OptionalGraphics }
-- AboutMods[modname] might be nil if there is no About\\*.txt
-- AboutMods[modname] = {Cat,Text,}
-- AboutMods[modname].Cat[category].Paragraph
-- category: Name$, Text$, Hint$, LongHint$
-- Name$, Text$, Hint$, LongHint$, Content$, Graphic$ 
-- finally, structure of file - Name$, Text$, Hint$, LongHint$, Content$, Graphic$ 

local function LoadAboutFile(mod, filepath)
	local cur_category = nil
	for _, t in ipairs(LoadTextTable(filepath)) do
		if not t.Name or t.Name == "" then
			-- Do nothing
		elseif (tostring(t.Name) == "Enabled:") then
			AboutMods[mod].Text = t.Text or mod
			AboutMods[mod].Hint = t.Hint
			AboutMods[mod].LongHint = t.LongHint
		elseif(tostring(t.Name) == "category:") then
			cur_category = tostring(t.Text)
			if(AboutMods[mod].Cat[cur_category] == nil) then 
				AboutMods[mod].Cat[cur_category] = {
					["Hint"] = tostring(t.Hint),
					["LongHint"] = tostring(t.LongHint),
					["Paragraph"] = {},
				} 
			end
		else
			if(cur_category ~= nil) then
				if string_find(t.Name, "[%.\r\n\"%z]") or string_sub(t.Name, 1, 1) == "-" then
					error("Illigal paragraph name: "..tostring(t.Name))
				elseif(AboutMods[mod].Cat[cur_category].Paragraph[tostring(t.Name)] ~= nil) then
					error("Paragraph already exist: "..tostring(t.Name))
				else
					AboutMods[mod].Cat[cur_category].Paragraph[tostring(t.Name)] = {
						["Text"] = t.Text,
						["Hint"] = t.Hint,
						["LongHint"] = t.LongHint,
						["Content"] = t.Content,
						["Graphic"] = t.Graphic,
					}
				end 
			else
				error("Trying to load paragraph without declared category. Mod: "..mod)
			end
		end
	end
end

function internal.LoadModAbout(mod, path)
	for f in path_find(path.."About\\*.txt") do
		if(AboutMods[mod] == nil) then AboutMods[mod] = { Cat = {}, Text = mod} end
		LoadAboutFile(mod, f)
	end
end
