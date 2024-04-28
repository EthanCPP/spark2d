-- print("HELLO FROM BOB!");

local entity = spark:entity()

-- print("start")

-- for k,v in pairs(entity.props) do
--     print(k .. "," .. v)
-- end

-- print("bob says armour is " .. entity.props.armour)

entity.props.armour = "yeet"
entity.props.alive = false
entity.props.pack = 69

-- print(entity.x)

-- print("want some props")
-- print(entity.props.armour)

function spark:update()
    if spark.keyboard:down(spark.keyboard.code.w) then
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
end

function spark:keypressed()
-- print("want some props")
-- print(entity.props.armour)
end

-- function dump(o)
--     if type(o) == 'table' then
--        local s = '{ '
--        for k,v in pairs(o) do
--           if type(k) ~= 'number' then k = '"'..k..'"' end
--           s = s .. '['..k..'] = ' .. dump(v) .. ','
--        end
--        return s .. '} '
--     else
--        return tostring(o)
--     end
--  end

--  -- sadly this returns nil :()
--  print("ALL PROPS", dump(entity.props))
