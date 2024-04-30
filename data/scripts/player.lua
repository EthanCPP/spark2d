local entity = spark:entity()
local playerSprite = entity:getSpriteComponent("body")
local jamSprite = entity:getSpriteComponent("jam")

function spark:update()
    -- Entity movement
    if spark.keyboard:down(spark.keyboard.code.w) then
        print("UP")
        entity.y = entity.y - (300 * spark.time.delta)
    end
    
    if spark.keyboard:down(spark.keyboard.code.s) then
        entity.y = entity.y + (300 * spark.time.delta)
    end
    
    if spark.keyboard:down(spark.keyboard.code.a) then
        entity.x = entity.x - (300 * spark.time.delta)
    end
    
    if spark.keyboard:down(spark.keyboard.code.d) then
        entity.x = entity.x + (300 * spark.time.delta)
    end

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