local entity = spark:entity()
local playerSprite = entity:getSpriteComponent("body")

local scene = spark:getScene("game")

local collider4 = scene:getEntity("collider4")

function spark:update()

    for i, v in pairs(entity:getColliders()) do
        print(v.props.isFloor)
    end

    if spark.keyboard:down(spark.keyboard.code.a) then
        if (entity.vx > -450) then
            entity:applyVelocity(-1, 0)
        end
    end
    
    if spark.keyboard:down(spark.keyboard.code.d) then
        if (entity.vx < 450) then
            entity:applyVelocity(1, 0)
        end
    end
    
    if spark.keyboard:down(spark.keyboard.code.w) then
        if (entity.vy > -450) then
            entity:applyVelocity(0, -1)
        end
    end
    
    if spark.keyboard:down(spark.keyboard.code.s) then
        if (entity.vy < 450) then
            entity:applyVelocity(0, 1)
        end
    end
end

function spark:keypressed(code)
    if code == spark.keyboard.code.enter then
        entity.static = not entity.static
    end

    if code == spark.keyboard.code.space then
        entity.vy = -1000
    end

    if (code == spark.keyboard.code.i) then
        collider4.x = collider4.x + 20
    end
end
