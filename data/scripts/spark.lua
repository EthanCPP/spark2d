-- System settings
spark:setWindowSize(1920, 1080)
spark:setWindowTitle("Ethan's game")
spark:setFramerateLimit(0)
spark:regenerateWindow()

-- Create a new scene called "game"
local scene = Scene.new("game")

-- Create a new entity called "bob" in our scene
local bob = Entity.new("game", "bob")
-- local bob = scene:createEntity("bob")

-- Set the "bob" entity position
-- bob.x = 96
-- bob.y = 40

bob.x = 50

bob.props.pack = 3
bob.props.armour = "100"
bob.props.alive = true
-- print("Pack: " .. bob.props.pack)
-- print("Armour: " .. bob.props.armour)

if (bob.props.alive) then
    print("alive")
else
    print("dead")
end

-- bob.props.health = 100

-- Create a new sprite component called "body"
-- local bobSprite = SpriteComponent.new("body", "images/bob.png")

-- Add the sprite component to our "bob" entity
local bobSprite = bob:addSpriteComponent("body", "images/bob.png")
bob:addScript("bob.lua")

-- Finally start the scene
scene:start()

bobSprite.x = 50
bobSprite.rotation = 12




-- Called every frame
function spark:update()
end

-- c = coroutine.wrap(function()
--     print("Coroutine start")
--     spark.time:wait(c, 2)
--     print("been 2 secs")
--     spark.time:wait(c, 2)
--     print("been another 2 secs")

--     local jam = scene:createEntity("jam")
--     jam.x = 40
--     jam.y = 20

--     local jamSprite = SpriteComponent.new("body", "images/jam.png")
--     jam:addSpriteComponent(jamSprite)
--     jam:addScript("jam.lua")
-- end)
-- c()


-- d = coroutine.wrap(function()
--     print("another 1 starting now")
--     spark.time:wait(d, 3)
--     print("another 1: been 3 secs")
--     spark.time:wait(d, 0.5)
--     print("another 1: been 0.5 secs")
-- end)
-- d()

function spark:keypressed(code)
    if (code == spark.keyboard.code.b) then
        print("Pack: " .. bob.props.pack)
        print("Armour: " .. bob.props.armour)
        
        if (bob.props.alive) then
            print("alive")
        else
            print("dead")
        end
    end

    if (code == spark.keyboard.code.c) then
        c = coroutine.wrap(function()
            local aa = bob:getSpriteComponent("body")
            aa.x = 120
            aa.y = 230

            spark.time:wait(c, 1)

            bobSprite.x = -30
            bobSprite.y = -30

        end)
        c()

    end

    if (code == spark.keyboard.code.r) then
        bob:getSpriteComponent("body").rotation = bob:getSpriteComponent("body").rotation + 10
    end

    if (code == spark.keyboard.code.g) then
        bob.rotation = bob.rotation+ 10
    end
end