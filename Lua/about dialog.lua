
local _G = _G
local internal = debug.getregistry()

local msg = debug.Message
local floor = math.floor
local RGB = color.RGB
local print = print
local Message = Message
local type = type
local unpack = unpack
local error = error
local pairs = pairs
local sortpairs = sortpairs
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
local format = string.format
local string_lower = string.lower
local string_find = string.find
local table_copy = table.copy
local table_concat = table.concat
local ScreenWidth = ScreenWidth
local ScreenHeight = ScreenHeight
local GetKeyState = internal.GetKeyState

local d_dialogs = dialogs
local d_new = dialogs.new
local d_Text = dialogs.Text
local d_Button = dialogs.Button
local d_Pcx = dialogs.Pcx
local d_ScrollBar = dialogs.ScrollBar
local d_AlignH = dialogs.AlignH
local d_AlignV = dialogs.AlignV
local d_AlignLayers = dialogs.AlignLayers
local d_CheckGroup = dialogs.CheckGroup
local d_CheckBox = dialogs.CheckBox
local d_Frame = dialogs.Frame
local d_MainMenuButton = dialogs.MainMenuButton

local TEXT = internal.TEXT
local dofile = dofile

----------- No globals from this point ------------

local _NOGLOBALS

---------------------------------------------------

-- here we can create dialog option for "About"
-- What can be inside? Informations about features, ERM/Lua language
-- how into modding and more. With mod support ofc, similar to WoG Options dialog
-- Shouldn't be too complex though, must have localisation support

local function BtnClick(it, cmd)
	
end

function internal.events.ShowDialog(t)
	local info = t.ClassPtr == 0x641720
	if info or t.ClassPtr == 0x641CBC then
		t:Add(d_MainMenuButton{Name = "About", X = (info and 619 or 622), Y = (info and 75 or 81), File = "ZWogOpt.def", OnClick = BtnClick})
	end
end
