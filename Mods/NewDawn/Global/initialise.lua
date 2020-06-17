-- by Jakub Grzana
-- initialise Cosmetic Enhancements

global[ModName] = {}

function global.events.EnterMap()
	global[ModName].DwHint = Options.DwHint
	global[ModName].SrHint = Options.SrHint
	global[ModName].WhHint = Options.WhHint
	global[ModName].ArtHint = Options.ArtHint
	global[ModName].QuickCombat = Options.QuickCombat
	global[ModName].BattlePic = Options.BattlePic
	global[ModName].NewMp = Options.NewMp
	global[ModName].NewSn = Options.NewSn
end
