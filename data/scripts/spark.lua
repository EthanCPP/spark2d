-- Create the scene
local scene = Scene.new("game")
scene:start()

-- Create the background
local eBackground = scene:createEntity("background")
eBackground:addSpriteComponent("background_sprite", "images/flappy/background-day.png")
eBackground.static = true

-- Create the floor
local eFloor = scene:createEntity("floor")
eFloor:addSpriteComponent("floor_sprite", "images/flappy/base.png")
eFloor.static = true
eFloor.y = 450
eFloor.props.kills = true
eFloor:setColliderSize(336, 112)

-- Create the bird
local eBird = scene:createEntity("bird")
eBird:addSpriteComponent("bird_sprite", "images/flappy/redbird-downflap.png")
eBird.y = 200
eBird.x = 50
eBird.props.dead = false
eBird.props.score = 0
eBird:setColliderSize(34, 24)
eBird:addScript("bird.lua")

-- Create a pipe
local ePipe1Top = scene:createEntity("pipe1_top")
local ePipe1TopSpr = ePipe1Top:addSpriteComponent("pipe1_top_sprite", "images/flappy/pipe-green.png")
ePipe1Top.static = true
ePipe1Top.x = 180
ePipe1Top.y = -100
ePipe1Top:setColliderSize(52, 320)
ePipe1Top.props.kills = true
ePipe1Top.props.pipe = "top"

ePipe1TopSpr.x = 52
ePipe1TopSpr.y = 320
ePipe1TopSpr.rotation = 180

ePipe1Top:addScript("pipe.lua")

local ePipe1Bottom = scene:createEntity("pipe1_bottom")
local ePipe1BottomSpr = ePipe1Bottom:addSpriteComponent("pipe1_bottom_sprite", "images/flappy/pipe-green.png")
ePipe1Bottom.static = true
ePipe1Bottom.x = 180
ePipe1Bottom.y = 360
ePipe1Bottom:setColliderSize(52, 320)
ePipe1Bottom.props.kills = true
ePipe1Bottom.props.pipe = "bottom"

ePipe1Bottom:addScript("pipe.lua")


-- Create the "fruit"
local eFruit = scene:createEntity("fruit")
eFruit.x = 180
eFruit.y = 0
eFruit.static = true
eFruit.props.eaten = false
eFruit:setColliderSize(40, 512)