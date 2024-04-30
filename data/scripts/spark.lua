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

-- Finally start the scene
scene:start()

c = coroutine.wrap(function()
    for i = 3, 1, -1 do
        print("Destroying in " .. i)
        spark.time:wait(c, 1)
    end

    player:destroy()
end)
c()