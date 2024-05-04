
local entity = spark:entity()
local sprite = entity:getSpriteComponent("bird_sprite")

function spark:update()
    for key, collider in pairs(entity:getColliders()) do
        if collider.props.kills then
            -- we have collided with an object that kills us
            entity.props.dead = true
        end

        if key == "fruit" and not collider.props.eaten and not entity.props.dead then
            collider.props.eaten = true
            entity.props.score = entity.props.score + 1
            print("points: " .. entity.props.score)
        end
    end

    if spark.keyboard:down(spark.keyboard.code.l) then
        sprite.rotation = sprite.rotation + 1
    end
    
    if spark.keyboard:down(spark.keyboard.code.k) then
        sprite.rotation = sprite.rotation - 1
    end
end

function spark:keypressed(code)
    if not entity.props.dead then
        if code == spark.keyboard.code.space then
            -- jump!
            entity.vy = -500
        end
    end
end