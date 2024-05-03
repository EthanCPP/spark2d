local entity = spark:entity()
local playerSprite = entity:getSpriteComponent("body")

function spark:update()
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
        if entity.vy > 0 then
            -- if falling, apply a lot of velocity
            entity:applyVelocity(0, -2000)
        elseif entity.vy < 0 then
            -- jumping, apply less velocity
            entity:applyVelocity(0, -500)
        else
            -- stationary
            entity:applyVelocity(0, -1000)
        end
    end
end
