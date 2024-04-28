#pragma once

#include "../Components/Interface/IComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/Transform.h"

#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>
#include <map>
#include <string>
#include <unordered_map>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

struct GameEntityPropItem
{
    sol::type type;

    std::string valueString;
    float valueNumber;
    bool valueBool;
};

struct GameEntityProperties
{
    sol::object getPropertyLua(const char* name, sol::this_state s)
    {
        sol::type type = props[name].type;
        sol::object object;

        switch (type)
        {
        case sol::type::string:
            object = sol::make_object(s, props[name].valueString);
            break;
        case sol::type::number:
            object = sol::make_object(s, props[name].valueNumber);
            break;
        case sol::type::boolean:
            object = sol::make_object(s, props[name].valueBool);
            break;
        default:
            object = sol::make_object(s, sol::nil);
            break;
        }

        return object;
    }

    void setPropertyLua(const char* name, sol::stack_object object)
    {
        GameEntityPropItem propItem;
        propItem.type = object.get_type();

        switch (propItem.type)
        {
        case sol::type::string:
            propItem.valueString = object.as<std::string>();
            break;
        case sol::type::number:
            propItem.valueNumber = object.as<float>();
            break;
        case sol::type::boolean:
            propItem.valueBool = object.as<bool>();
            break;
        }

        props[name] = propItem;
    }

    std::unordered_map<std::string, GameEntityPropItem> props;
};

class GameEntity
{
public:
    GameEntity();
    ~GameEntity();

    void addSpriteComponent(std::shared_ptr<SpriteComponent> spriteComponent);

    std::shared_ptr<IComponent> getComponent(std::string key);

    void update();
    void render(std::shared_ptr<sf::RenderWindow> window);

    float getX();
    float getY();
    float getRotation();

    void setX(float x);
    void setY(float y);
    void setRotation(float angle);

    GameEntityProperties& getDynamicProps();

    std::string key;
    std::string sceneKey;
private:
    std::map<std::string, std::shared_ptr<IComponent>> mComponents;

    Transform transform;

    GameEntityProperties mDynamicProps;
};