#include "LuaRectangleComponent.h"


LuaRectangleComponent::LuaRectangleComponent(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals)
{
    mSceneManager = sceneManager;
    mWindow = window;
    mGlobals = globals;
}

LuaRectangleComponent::~LuaRectangleComponent()
{

}

void LuaRectangleComponent::setup(sol::state& lua)
{
    /*
    * =========================================
    * Create a new rectangle component object
    *
    * DON'T CREATE COMPONENT THIS WAY. USE (Entity):addRectangleComponent(key)
    * =========================================
    */
    lua.new_usertype<RectangleComponent>("RectangleComponent",
        sol::meta_function::construct,
        sol::factories(
            [this](std::string key) {
                std::cout << "Please use (entity):addRectangleComponent(key, path) instead of RectangleComponent.new()!" << std::endl;
                std::shared_ptr<RectangleComponent> rectangleComponent = std::make_shared<RectangleComponent>(key);

                return rectangleComponent;
            }
        ),
        "x", sol::property(&RectangleComponent::getX, &RectangleComponent::setX),
                "y", sol::property(&RectangleComponent::getY, &RectangleComponent::setY),
                "rotation", sol::property(&RectangleComponent::getRotation, &RectangleComponent::setRotation)
                );

    lua["RectangleComponent"]["setSize"] = [this](RectangleComponent& component, float width, float height)
    {
        component.setSize(width, height);
    };

    lua["RectangleComponent"]["setColour"] = [this](RectangleComponent& component, int r, int g, int b)
    {
        component.setFillColour(r, g, b, 255);
    };

    lua["RectangleComponent"]["setColourAlpha"] = [this](RectangleComponent& component, int r, int g, int b, int a)
    {
        component.setFillColour(r, g, b, a);
    };

    lua["RectangleComponent"]["setOutlineThickness"] = [this](RectangleComponent& component, float thickness)
    {
        component.setOutlineThickness(thickness);
    };

    lua["RectangleComponent"]["setOutlineColour"] = [this](RectangleComponent& component, int r, int g, int b)
    {
        component.setOutlineColour(r, g, b, 255);
    };

    lua["RectangleComponent"]["setOutlineColourAlpha"] = [this](RectangleComponent& component, int r, int g, int b, int a)
    {
        component.setOutlineColour(r, g, b, a);
    };

    lua["RectangleComponent"]["setTexture"] = [this](RectangleComponent& component, std::string filepath)
    {
        component.setTexture(filepath);
    };
}