-- print("jam says hello mate")

local entity = spark:entity()

function spark:update()
    if spark.keyboard:down(spark.keyboard.code.w) then
        entity.y = entity.y - 0.2
    end
    
    if spark.keyboard:down(spark.keyboard.code.s) then
        entity.y = entity.y + 0.2
    end
    
    if spark.keyboard:down(spark.keyboard.code.a) then
        entity.x = entity.x - 0.2
    end
    
    if spark.keyboard:down(spark.keyboard.code.d) then
        entity.x = entity.x + 0.2
    end
end
