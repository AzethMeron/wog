
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
local AboutMods = internal.AboutMods

----------- No globals from this point ------------

local _NOGLOBALS

---------------------------------------------------

local BlueStyle = nil
local FillColor = BlueStyle and RGB(20, 20, 54) or RGB(102, 72, 54)
local BorderLColor = BlueStyle and RGB(99, 113, 173) or 33574
local BorderSColor = BlueStyle and RGB(16, 32, 74) or 10434
local ItemFont --= "Small.fnt"
local CaptionFont = "MedFont.fnt"
local DlgF = {mod = "wog", cat = 1}

local function BuildDlg()
	return d_new(table_copy(DlgF, {ExpandWidth = 1, ExpandHeight = 1,
		Border = true,
		BorderHint = true,
		PlayerColor = BlueStyle and 1,
		ShowLongHints = true,
		DropShadow = false,
		BackgroundPcx = BlueStyle and "DiBoxBlu.pcx",

		OnKeyUp = function(t, cmd)
			cmd.CloseDialog = cmd.Key == 1
		end,
		
		d_AlignV{Name = "Outer",
			Width = ScreenWidth, Height = ScreenHeight, Margin = 25, SpaceY = 10, MarginTop = 28, MarginBottom = 45,
			AlignX = 0.5, AlignY = 0.5,

			-- d_Text{Text = "WoGify Options", Font = "BigFont.fnt", Color = 19},
			-- 12,
			d_AlignH{ExpandWidth = 1, ExpandHeight = 1, SpaceX = 6,
				
				d_AlignV{ExpandHeight = 1, SpaceX = 1, Width = 210, SpaceY = 4, AlignX = 0.5,
					d_Text{Name = "ModsCaption", Font = CaptionFont, Color = 19, ExpandWidth = 1,
						Text = TEXT.ModsText, Hint = TEXT.ModsHint, LongHint = TEXT.ModsLongHint,
					},  
					d_AlignH{ExpandWidth = 1,
						d_CheckGroup{Name = "ModsGroup", 
							BorderHeight = 8,
							-- Texts = {"In The Wake Of Gods", "Horn Of The Abyss", "Master Of Puppets"},
							Border = true,
							--CloseDialog = true,
							ExpandWidth = 1, MarginTop = -1, MarginBottom = -1,
							-- MinWidth = 0, ItemsAutoHeight = true,
							ScrollBar = "ScrollMods",
							Radio = true,
							FillColor = FillColor,
							BorderLColor = BorderLColor,
							BorderSColor = BorderSColor,
							Font = ItemFont,
							-- States = 1,
							-- OnClick = ModTagClick,
							OnClick = function(it)
								local dlg = it.Parent
								dlg.mod = dlg.mods[it.Index]
								dlg.NoRedraw = true
								dlg:updateCats()
								dlg.NoRedraw = false
								dlg:Redraw()
							end
						},
						d_ScrollBar{Name = "ScrollMods", ExpandHeight = 1, Blue = BlueStyle},
					},
					17,
					d_Text{Name = "CatsCaption", Font = CaptionFont, Color = 19, ExpandWidth = 1, Visible = false,
						Text = TEXT.CategoryText, Hint = TEXT.CategoryHint, LongHint = TEXT.CategoryLongHint,
					},  
					d_AlignH{ExpandWidth = 1, ExpandHeight = 1,
						d_CheckGroup{Name = "CatsGroup",
							BorderHeight = 8,
							-- Texts = {"All!", "Combat", "Interface", "Stupid", "Multiplayer"},
							Border = true,
							--CloseDialog = true,
							ExpandWidth = 1, MarginTop = -1, MarginBottom = -1,
							ScrollBar = "ScrollCats",
							Radio = true,
							FillColor = FillColor,
							BorderLColor = BorderLColor,
							BorderSColor = BorderSColor,
							Font = ItemFont,
							OnClick = function(it)
								local dlg = it.Parent
								dlg.cat = it.Index
								dlg.NoRedraw = true
								dlg:updateOptions()
								dlg.NoRedraw = false
								dlg:Redraw()
							end
							-- AllIndex = 1,
						},
						d_ScrollBar{Name = "ScrollCats", ExpandHeight = 1, Visible = false, Blue = BlueStyle},
					},
				},
				d_AlignV{Name = "OptionsOuter", ExpandWidth = 1, ExpandHeight = 1, MinWidth = 0,
					d_AlignH{Name = "Options",
						ExpandWidth = 1, ExpandHeight = 1, SpaceX = 3, SpaceY = 0,
						MinWidth = 0,
						ScrollBar = "Scroll",
						FullPageScroll = true,
					},
					d_ScrollBar{Name = "Scroll", CatchKeys = true, ExpandWidth = 1, Visible = false, Blue = BlueStyle},
				},
			},
			d_AlignH{SpaceX = 16,
				d_AlignLayers{AlignX = 0.5, AlignY = 0.5,
					d_Pcx{File = "box64x30.pcx"},
					d_Button{Name = "OK", File = "iokay.def", CloseDialog = true, Id = 30722, HotKey = 28},
				},
				d_AlignLayers{AlignX = 0.5, AlignY = 0.5,
					d_Pcx{File = "box64x30.pcx"},
					d_Button{File = "icancel.def", CloseDialog = true, Id = 30721, HotKey = 1},
				},
			},
		},
	}, true))
end

-- here we can create dialog option for "About"
-- What can be inside? Informations about features, ERM/Lua language
-- how into modding and more. With mod support ofc, similar to WoG Options dialog
-- Shouldn't be too complex though, must have localisation support

local function BtnClick(it, cmd)
	local dlg = BuildDlg()
	dlg.Items.Outer:NeedSize()
	dlg:Show() 
end

function internal.events.ShowDialog(t)
	local info = t.ClassPtr == 0x641720
	if info or t.ClassPtr == 0x641CBC then
		t:Add(d_MainMenuButton{Name = "About", X = (info and 619 or 622), Y = (info and 75 or 81), File = "ZWogOpt.def", OnClick = BtnClick})
	end
end
