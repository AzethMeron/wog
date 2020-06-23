-- by Jakub Grzana
-- initialise Cosmetic Enhancements

global[ModName] = {}

function global.events.EnterContext()
	global[ModName].DwHint = Options.DwHint
	global[ModName].SrHint = Options.SrHint
	global[ModName].WhHint = Options.WhHint
	global[ModName].ArtHint = Options.ArtHint
	global[ModName].QuickCombat = Options.QuickCombat
	global[ModName].BattlePic = Options.BattlePic
	global[ModName].NewMp = Options.NewMp
	global[ModName].NewSn = { Enabled = Options.NewSn, Replace = {}, Object = {} , ShutUp = {} }
	
	global[ModName].FasterCombat = Options.FasterCombat
	global[ModName].FasterAdvMap = Options.FasterAdvMap
	global[ModName].FizzleFade = Options.FizzleFade
	global[ModName].Trees = Options.Trees
	global[ModName].NewGrid = Options.NewGrid
	global[ModName].MonStandAnim = Options.MonStandAnim
	global[ModName].SoftGridShadow = Options.SoftGridShadow
	global[ModName].SoftShadow = Options.SoftShadow
end
