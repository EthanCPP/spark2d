#pragma once

#include "../Components/Interface/IComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TextComponent.h"
#include "../Components/CircleComponent.h"
#include "../Components/RectangleComponent.h"
#include "../Components/Transform.h"
#include "ResourceManager.h"
#include "SparkGlobals.h"

#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>
#include <map>
#include <string>
#include <unordered_map>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

// Forward declaration of LuaApi due to mutual inclusion.
class LuaApi;

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

    void init(ResourceManager* resourceManager, std::shared_ptr<SparkGlobals> sparkGlobals);
    void addLuaState(std::shared_ptr<LuaApi> luaState);

    void addSpriteComponent(std::shared_ptr<SpriteComponent> spriteComponent);
    void addTextComponent(std::shared_ptr<TextComponent> textComponent);
    void addCircleComponent(std::shared_ptr<CircleComponent> circleComponent);
    void addRectangleComponent(std::shared_ptr<RectangleComponent> rectangleComponent);

    std::shared_ptr<IComponent> getComponent(std::string key);

    void update(float dt);
    void render(std::shared_ptr<sf::RenderWindow> window);

    float getX();
    float getY();
    float getRotation();

    void setX(float x);
    void setY(float y);
    void setRotation(float angle);

    bool getStatic();
    void setStatic(bool bStatic);

    void applyVelocity(sf::Vector2f velocity);
    float getXVelocity();
    float getYVelocity();
    void setXVelocity(float x);
    void setYVelocity(float y);

    void setColliderSize(float width, float height);
    void setColliderOffset(float x, float y);
    void setColliderDebug(bool debug);
    bool getColliderDebug();
    sf::FloatRect getCollider();

    void setSolid(bool solid);
    bool getSolid();

    bool checkCollision(std::shared_ptr<GameEntity> entity);

    GameEntityProperties& getDynamicProps();

    std::string key;
    std::string sceneKey;

    std::vector<std::shared_ptr<LuaApi>> mLuaStates;


    sf::Vector2f mVelocity;
    std::vector<std::shared_ptr<GameEntity>> mColliders;
    bool mSolid; // whether a colliding object should debounce
private:
    std::map<std::string, std::shared_ptr<IComponent>> mComponents;

    Transform transform;
    ResourceManager* mResourceManager;
    std::shared_ptr<SparkGlobals> mSparkGlobals;

    GameEntityProperties mDynamicProps;

private:
    // Physics
    bool mStatic;
    sf::RectangleShape mColliderDebug;
    bool mShowColliderDebug;
    sf::FloatRect mCollider;
};