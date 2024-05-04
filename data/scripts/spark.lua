-- Create a new scene called "game"
local scene = Scene.new("game")

-- Add a new entity named 'player' to our scene
local player = scene:createEntity("player")
player.x = 200
player.y = 200

player:setColliderSize(32, 32)
player.debug = true
-- player.static = true -- do not apply gravity


-- Add the sprite component to our "bob" entity
local playerSprite = player:addSpriteComponent("body", "images/bob.png")


-- Add a floor
local floor = scene:createEntity("floor")
floor.x = 0
floor.y = 1000;
floor.static = true
floor.debug = true
floor:setColliderSize(1920, 40)

local floorRect = floor:addRectangleComponent("floorRect")
floorRect:setSize(1920, 40)
floorRect:setColour(120, 120, 240)

local collider1 = scene:createEntity("collider1")
collider1.x = 2
collider1.y = 0
collider1.static = true
collider1.debug = true
collider1.props.isFloor = true
collider1:setColliderSize(50, 1080)

local collider2 = scene:createEntity("collider2")
collider2.x = 1900
collider2.y = 0
collider2.static = true
collider2.debug = true
collider2.props.isFloor = true
collider2:setColliderSize(50, 1080)

local collider3 = scene:createEntity("collider3")
collider3.x = 3
collider3.y = 3
collider3.static = true
collider3.debug = true
collider3.props.isFloor = true
collider3:setColliderSize(1920, 30)

local collider4 = scene:createEntity("collider4")
collider4.x = 540
collider4.y = 320
collider4.static = true
collider4.debug = true
collider4.solid = false
collider4.props.isFloor = false
collider4:setColliderSize(100, 150)

-- Attach the player script
player:addScript("player.lua")

-- Finally start the scene
scene:start()

