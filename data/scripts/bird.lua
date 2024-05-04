
local entity = spark:entity()
local sprite = entity:getSpriteComponent("bird_sprite")

local soundFlap = entity:addSoundComponent("flap", "audio/wing.ogg")
soundFlap.volume = 80

local soundHit = entity:addSoundComponent("hit", "audio/hit.ogg")
soundHit.volume = 60

local soundDie = entity:addSoundComponent("die", "audio/die.ogg")
local soundPoint = entity:addSoundComponent("point", "audio/point.ogg")
soundPoint.volume = 50

function setRotation()
    if entity.props.dead then
        sprite.rotation = 90
        return
    end

    if entity.vy < -1 then
        sprite.rotation = (entity.vy / 500) * 60
    elseif entity.vy > 1 then
        local rotation = (entity.vy / 500) * 60

        if rotation > 90 then
            rotation = 90
        end

        sprite.rotation = rotation
    end
end

function spark:update()
    setRotation()

    if not entity.props.dead then
        for key, collider in pairs(entity:getColliders()) do
            if collider.props.kills then
                -- we have collided with an object that kills us
                entity.props.dead = true
                soundHit:play()
                soundDie:play()
            end

            if collider.props.fruit and not collider.props.eaten and not entity.props.dead then
                collider.props.eaten = true
                entity.props.score = entity.props.score + 1
                soundPoint:play()
                print("points: " .. entity.props.score)
            end
        end
    end
end

function spark:keypressed(code)
    if not entity.props.dead then
        if code == spark.keyboard.code.c then
            -- jump!
            entity.vy = -420
            soundFlap:play()
        end
    end
end
