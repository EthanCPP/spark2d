local scene = spark:getScene("game")
local entity = spark:entity()

local distance = 540

local bird = scene:getEntity("bird")
local fruit = scene:getEntity("fruit")
local fruit2 = scene:getEntity("fruit2")

function set()
    if entity.props.pipe == "top" then
        if bird.props.score >= 5 then
            distance = 500
        end

        if bird.props.score >= 10 then
            distance = 480
        end

        if bird.props.score >= 20 then
            distance = 440
        end

        local posYTop = math.random(-280, -50)
        local posYBottom = posYTop + distance

        entity.y = posYTop

        if entity.props.pipenumber == 1 then
            local bottomPipe = scene:getEntity("pipe1_bottom")
            bottomPipe.y = posYBottom
        else
            local bottomPipe = scene:getEntity("pipe2_bottom")
            bottomPipe.y = posYBottom
        end
    end
end

set()

function spark:update()
    if not bird.props.dead then
        local speed = 100 + (bird.props.score * 5)
        entity.x = entity.x - (speed * spark.time.delta)
        
        if entity.props.pipenumber == 1 then
            fruit.x = entity.x + 5
        else
            fruit2.x = entity.x + 5
        end

        if entity.x < -52 then
            set()
            entity.x = 300

            if entity.props.pipenumber == 1 then
                fruit.props.eaten = false
            else
                fruit2.props.eaten = false
            end
        end
    end
end