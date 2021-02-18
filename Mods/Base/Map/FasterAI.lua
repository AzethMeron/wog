-- by GrayFace

-- Sets thinking radius to WoG 3.58 default (4096) instead of SoD default (32000).
-- WoG 3.59 normally uses SoD default.

if new and Options.FasterAI then
	UN:J4(0x1000)
end

i--[[f new then
	VA(1):V("old sport",4)
	VA(1):V("old sport", 0, 79)
	VA(1):V("old sport", 1, 71)
	VA(1):V("old sport", 2, 73)
	VA(1):V("old sport", 3, 74)
end

-- stored
--local addr, length = VA(0):C("old sport",?v,?v)
--Message("Stored old sport: "..addr.." "..length)
--local addr1, length1 = VA(1):C("old sport",?v,?v)
--Message("Temp old sport: "..addr1.." "..length1)

--local ad1, ad2 = VA(0):C("old sport",?v), VA(0):A("old sport",?ptr)
--Message(ad1.." "..ad2)


Message(VA(1):A("old sport",?z))
--]] 
