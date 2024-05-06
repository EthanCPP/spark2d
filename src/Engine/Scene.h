#pragma once
#include "ResourceManager.h"
#include "EntityManager.h"
#include "GuiManager.h"
#include "SparkGlobals.h"

#include <SFML/Graphics.hpp>

#include <memory>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

// TODO move this elseware
struct ScenePropItem
{
    sol::type type;

    std::string valueString;
    float valueNumber;
    bool valueBool;
};

struct SceneProperties
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
        ScenePropItem propItem;
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

    std::unordered_map<std::string, ScenePropItem> props;
};

class Scene
{
public:
	Scene(std::string key);
	~Scene();

	void init(ResourceManager* resourceManager, std::shared_ptr<SparkGlobals> sparkGlobals);
	void start();
	void stop();
	void update(float dt);
	void render(std::shared_ptr<sf::RenderWindow> window);

    SceneProperties& getDynamicProps();

	std::shared_ptr<EntityManager> entityManager;
	std::shared_ptr<GuiManager> guiManager;

	std::string key;

private:
	bool mInitialised;

    SceneProperties mDynamicProps;

};

