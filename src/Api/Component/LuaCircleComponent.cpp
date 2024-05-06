#include "LuaCircleComponent.h"


LuaCircleComponent::LuaCircleComponent(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals)
{
    mSceneManager = sceneManager;
    mWindow = window;
    mGlobals = globals;
}

LuaCircleComponent::~LuaCircleComponent()
{

}

void LuaCircleComponent::setup(sol::state& lua)
{
    /*
    * =========================================
    * Create a new circle component object
    *
    * DON'T CREATE COMPONENT THIS WAY. USE (Entity):addCircleComponent(key)
    * =========================================
    */
    lua.new_usertype<CircleComponent>("CircleComponent",
        sol::meta_function::construct,
        sol::factories(
            [this](std::string key) {
                std::cout << "Please use (entity):addCircleComponent(key, path) instead of CircleComponent.new()!" << std::endl;
                std::shared_ptr<CircleComponent> circleComponent = std::make_shared<CircleComponent>(key);

                return circleComponent;
            }
        ),
        "x", sol::property(&CircleComponent::getX, &CircleComponent::setX),
                "y", sol::property(&CircleComponent::getY, &CircleComponent::setY),
                "rotation", sol::property(&CircleComponent::getRotation, &CircleComponent::setRotation)
                );

    lua["CircleComponent"]["setRadius"] = [this](CircleComponent& component, float radius)
    {
        component.setRadius(radius);
    };

    lua["CircleComponent"]["setColour"] = [this](CircleComponent& component, int r, int g, int b)
    {
        component.setFillColour(r, g, b, 255);
    };

    lua["CircleComponent"]["setColourAlpha"] = [this](CircleComponent& component, int r, int g, int b, int a)
    {
        component.setFillColour(r, g, b, a);
    };

    lua["CircleComponent"]["setOutlineThickness"] = [this](CircleComponent& component, float thickness)
    {
        component.setOutlineThickness(thickness);
    };

    lua["CircleComponent"]["setOutlineColour"] = [this](CircleComponent& component, int r, int g, int b)
    {
        component.setOutlineColour(r, g, b, 255);
    };

    lua["CircleComponent"]["setOutlineColourAlpha"] = [this](CircleComponent& component, int r, int g, int b, int a)
    {
        component.setOutlineColour(r, g, b, a);
    };

    lua["CircleComponent"]["setTexture"] = [this](CircleComponent& component, std::string filepath)
    {
        component.setTexture(filepath);
    };
}