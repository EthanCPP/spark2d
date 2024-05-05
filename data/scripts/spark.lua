-- Create the scene
local scene = Scene.new("game")
scene:start()

-- Create the background
local eBackground = scene:createEntity("background")
local bgSpr1 = eBackground:addSpriteComponent("background_sprite", "images/flappy/background-day.png")
local bgSpr2 = eBackground:addSpriteComponent("background_sprite2", "images/flappy/background-day.png")
bgSpr2.x = bgSpr1:getSize().width
eBackground.static = true
eBackground.zindex = -1

-- Create the floor
local eFloor = scene:createEntity("floor")
local floorSpr1 = eFloor:addSpriteComponent("floor_sprite", "images/flappy/base.png")
local floorSpr2 = eFloor:addSpriteComponent("floor_sprite2", "images/flappy/base.png")
floorSpr2.x = floorSpr1:getSize().width
eFloor.static = true
eFloor.y = 450
eFloor.props.kills = true
eFloor:setColliderSize(336, 112)
eFloor.zindex = 1

-- Create the bird
local eBird = scene:createEntity("bird")

local eBirdSprite = eBird:addSpriteComponent("bird_sprite", "images/flappy/flappy.png")
eBirdSprite:setFrameDimensions(34, 24)


eBird.y = 200
eBird.x = 50
eBird.props.ready = true
eBird.props.dead = true
eBird.static = true
eBird.props.score = 0
eBirdSprite:setOrigin(17, 14)
eBird:setColliderSize(18, 10)
eBird:setColliderOffset(-10, -5)
eBird:addScript("bird.lua")
eBird.zindex = 2

-- Create a pipe
local ePipe1Top = scene:createEntity("pipe1_top")
local ePipe1TopSpr = ePipe1Top:addSpriteComponent("pipe1_top_sprite", "images/flappy/pipe-green.png")
ePipe1Top.static = true
ePipe1Top.x = 180
ePipe1Top.y = -100
ePipe1Top:setColliderSize(52, 320)
ePipe1Top.props.kills = true
ePipe1Top.solid = false
ePipe1Top.props.pipe = "top"
ePipe1Top.props.pipenumber = 1

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
ePipe1Bottom.solid = false
ePipe1Bottom.props.pipe = "bottom"

ePipe1Bottom:addScript("pipe.lua")

-- Create a pipe
local ePipe2Top = scene:createEntity("pipe2_top")
local ePipe2TopSpr = ePipe2Top:addSpriteComponent("pipe2_top_sprite", "images/flappy/pipe-green.png")
ePipe2Top.static = true
ePipe2Top.x = 349
ePipe2Top.y = -100
ePipe2Top:setColliderSize(52, 320)
ePipe2Top.props.kills = true
ePipe2Top.solid = false
ePipe2Top.props.pipe = "top"
ePipe2Top.props.pipenumber = 2

ePipe2TopSpr.x = 52
ePipe2TopSpr.y = 320
ePipe2TopSpr.rotation = 180

ePipe2Top:addScript("pipe.lua")

local ePipe2Bottom = scene:createEntity("pipe2_bottom")
local ePipe2BottomSpr = ePipe2Bottom:addSpriteComponent("pipe2_bottom_sprite", "images/flappy/pipe-green.png")
ePipe2Bottom.static = true
ePipe2Bottom.x = 349
ePipe2Bottom.y = 360
ePipe2Bottom:setColliderSize(52, 320)
ePipe2Bottom.props.kills = true
ePipe2Bottom.solid = false
ePipe2Bottom.props.pipe = "bottom"

ePipe2Bottom:addScript("pipe.lua")


-- Create the "fruit"
local eFruit = scene:createEntity("fruit")
eFruit.x = 180
eFruit.y = 0
eFruit.static = true
eFruit.debug = false
eFruit.props.fruit = true
eFruit.props.eaten = false
eFruit:setColliderSize(40, 512)

-- Create the "fruit"
local eFruit2 = scene:createEntity("fruit2")
eFruit2.x = 349
eFruit2.y = 0
eFruit2.static = true
eFruit2.props.fruit = true
eFruit2.props.eaten = false
eFruit2:setColliderSize(40, 512)

-- GUI
local scoreGui = scene:createGuiEntity("score")
scoreGui.x = 144
scoreGui.y = 50

local scoreHundreds = scoreGui:addSpriteComponent("hundreds", "images/flappy/score_spritesheet.png")
scoreHundreds:setOrigin(12, 0)
scoreHundreds:setFrameDimensions(24, 36)
scoreHundreds.x = -1000 -- out of sight

local scoreTens = scoreGui:addSpriteComponent("tens", "images/flappy/score_spritesheet.png")
scoreTens:setOrigin(12, 0)
scoreTens:setFrameDimensions(24, 36)
scoreTens.x = -1000 -- out of sight

local scoreDigits = scoreGui:addSpriteComponent("digits", "images/flappy/score_spritesheet.png")
scoreDigits:setOrigin(12, 0)
scoreDigits:setFrameDimensions(24, 36)

function spark:update()
    if not eBird.props.dead then
        local speed = ePipe1Top.props.speed
        bgSpr1.x = bgSpr1.x - (speed * spark.time.delta)
        bgSpr2.x = bgSpr2.x - (speed * spark.time.delta)

        if (bgSpr1.x < -288) then
            bgSpr1.x = bgSpr2.x + 288
        end

        if (bgSpr2.x < -288) then
            bgSpr2.x = bgSpr1.x + 288
        end
        
        floorSpr1.x = floorSpr1.x - (speed * spark.time.delta)
        floorSpr2.x = floorSpr2.x - (speed * spark.time.delta)

        if (floorSpr1.x < -336) then
            floorSpr1.x = floorSpr2.x + 336
        end

        if (floorSpr2.x < -336) then
            floorSpr2.x = floorSpr1.x + 336
        end
    end

    local score = eBird.props.score

    if score - 100 >= 0 then
        scoreHundreds.x = -24
        scoreTens.x = 0
        scoreDigits.x = 24

        scoreHundreds:setFrame(math.floor(score / 100))
        scoreTens:setFrame(math.floor((score % 100) / 10))
        scoreDigits:setFrame(score % 10)
    elseif score - 10 >= 0 then
        scoreHundreds.x = -1000
        scoreTens.x = -12
        scoreDigits.x = 12

        scoreTens:setFrame(math.floor(score / 10))
        scoreDigits:setFrame(math.floor(score % 10))
    else
        scoreHundreds.x = -1000
        scoreTens.x = -1000
        scoreDigits.x = 0

        scoreDigits:setFrame(math.floor(score))
    end
end

function spark:keyreleased(code)
    if eBird.props.dead and code == spark.keyboard.code.enter then
        -- reset pipes
        ePipe1Top.x = 180
        ePipe1Top.y = -100
        ePipe1Bottom.x = 180
        ePipe1Bottom.y = 360

        ePipe2Top.x = 349
        ePipe2Top.y = -100
        ePipe2Bottom.x = 349
        ePipe2Bottom.y = 360

        -- reset fruit
        eFruit.props.eaten = false
        eFruit.x = 180
        eFruit2.props.eaten = false
        eFruit2.x = 349

        -- reset bird
        eBird.y = 200
        eBird.x = 50
        eBird.static = true
        eBird.props.score = 0
        eBird.props.ready = true
    end
end
