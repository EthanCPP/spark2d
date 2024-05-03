local entity = spark:entity()
local playerSprite = entity:getSpriteComponent("body")
local jamSprite = entity:getSpriteComponent("jam")

function spark:update()
    -- Entity movement
    -- if spark.keyboard:down(spark.keyboard.code.w) then
    --     print("UP")
    --     entity.y = entity.y - (300 * spark.time.delta)
    -- end
    
    -- if spark.keyboard:down(spark.keyboard.code.s) then
    --     entity.y = entity.y + (300 * spark.time.delta)
    -- end
    
    -- if spark.keyboard:down(spark.keyboard.code.a) then
    --     entity.x = entity.x - (300 * spark.time.delta)
    -- end
    
    -- if spark.keyboard:down(spark.keyboard.code.d) then
    --     entity.x = entity.x + (300 * spark.time.delta)
    -- end

    -- Entity movement w/ physics tests
    -- if spark.keyboard:down(spark.keyboard.code.w) then
    -- end
    
    -- if spark.keyboard:down(spark.keyboard.code.s) then
    -- end
    
    -- if spark.keyboard:down(spark.keyboard.code.a) then
    --     if entity.vx > -1000 then
    --         entity:applyVelocity(-1.5, 0)
    --     end
    -- elseif spark.keyboard:down(spark.keyboard.code.d) then
    --     if entity.vx < 1000 then
    --         entity:applyVelocity(1.5, 0)
    --     end
    -- else
    --     -- apply drag
    --     if (math.abs(entity.vx) < 10) then
    --         entity.vx = 0
    --     elseif (entity.vx < 0) then
    --         entity:applyVelocity(1, 0)
    --     else
    --         entity:applyVelocity(-1, 0)
    --     end
    -- end


    -- Sprite separation movement
    if spark.keyboard:down(spark.keyboard.code.up) then
        playerSprite.y = playerSprite.y - (50 * spark.time.delta)
    end
    
    if spark.keyboard:down(spark.keyboard.code.down) then
        playerSprite.y = playerSprite.y + (50 * spark.time.delta)
    end
    
    if spark.keyboard:down(spark.keyboard.code.left) then
        playerSprite.x = playerSprite.x - (50 * spark.time.delta)
    end
    
    if spark.keyboard:down(spark.keyboard.code.right) then
        playerSprite.x = playerSprite.x + (50 * spark.time.delta)
    end
    
    if spark.keyboard:down(spark.keyboard.code.g) then
        playerSprite.rotation = playerSprite.rotation + (100 * spark.time.delta)
    end

    -- Jam sprite separation movement
    if spark.keyboard:down(spark.keyboard.code.i) then
        jamSprite.y = jamSprite.y - (50 * spark.time.delta)
    end
    
    if spark.keyboard:down(spark.keyboard.code.k) then
        jamSprite.y = jamSprite.y + (50 * spark.time.delta)
    end
    
    if spark.keyboard:down(spark.keyboard.code.j) then
        jamSprite.x = jamSprite.x - (50 * spark.time.delta)
    end
    
    if spark.keyboard:down(spark.keyboard.code.l) then
        jamSprite.x = jamSprite.x + (50 * spark.time.delta)
    end
    
    -- Local rotation
    if spark.keyboard:down(spark.keyboard.code.g) then
        playerSprite.rotation = playerSprite.rotation + (100 * spark.time.delta)
    end
    
    -- Global rotation
    if spark.keyboard:down(spark.keyboard.code.h) then
        entity.rotation = entity.rotation + (100 * spark.time.delta)
    end
end

-- turn off gravity!
function spark:keypressed(code)
    if code == spark.keyboard.code.enter then
        entity.static = not entity.static
    end

    if code == spark.keyboard.code.space then
        if entity.vy > 0 then
            -- if falling, apply a lot of velocity
            entity:applyVelocity(0, -2000)
        else
            -- jumping, apply less velocity
            entity:applyVelocity(0, -500)
        end
    end
end

local nameText = entity:getTextComponent("name")
