-- by Jakub Grzana

local Lib = require "Lib"

-- Create table for global variables of mod
if new then
    _G[ModName] = {}
end

-- Create table for every square on the map
-- _G[ModName].map[x][y][l] = {}
if new then
    _G[ModName].map = {}
    local size, level = UN:X(?v,?v)
    for i=0,size-1,1 do
        _G[ModName].map[i]={}
        for j=0,size-1,1 do
            _G[ModName].map[i][j] ={}
            for l=0,level,1 do
                _G[ModName].map[i][j][l] = {}
                -- call init function for square
            end
        end
    end
end
