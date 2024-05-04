local scene = spark:getScene("game")
local entity = spark:entity()

local distance = 540

local bird = scene:getEntity("bird")
local fruit = scene:getEntity("fruit")

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

        local bottomPipe = scene:getEntity("pipe1_bottom")
        bottomPipe.y = posYBottom
    end
end

set()

function spark:update()
    local speed = 100 + (bird.props.score * 5)
    entity.x = entity.x - (speed * spark.time.delta)
    fruit.x = entity.x + 5

    if entity.x < -52 then
        set()
        entity.x = 300
        fruit.props.eaten = false
    end
end