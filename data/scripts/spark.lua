-- Create a new scene called "game"
local scene = Scene.new("game")

-- Add a new entity named 'player' to our scene
local player = scene:createEntity("player")
player.x = 200
player.y = 200

-- Attach the player script
player:addScript("player.lua")

-- Add the sprite component to our "bob" entity
local playerSprite = player:addSpriteComponent("body", "images/bob.png")
local jamSprite = player:addSpriteComponent("jam", "images/jam.png")

-- Add text
local playerText = player:addTextComponent("name", "fonts/comicsans.ttf")
playerText:setText("Ethan")
playerText:setCharacterSize(24)
playerText:setColour(255, 60, 60)
playerText:setBold(true)
playerText:setUnderlined(true)
playerText:setItalic(true)
playerText.x = -50
playerText.y = -50

-- Finally start the scene
scene:start()

