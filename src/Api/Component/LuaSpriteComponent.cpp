#include "LuaSpriteComponent.h"


LuaSpriteComponent::LuaSpriteComponent(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals)
{
    mSceneManager = sceneManager;
    mWindow = window;
    mGlobals = globals;
}

LuaSpriteComponent::~LuaSpriteComponent()
{

}

void LuaSpriteComponent::setup(sol::state& lua)
{
    /*
    * =========================================
    * Create a new sprite component object with desired texture
    *
    * DON'T CREATE SPRITECOMPONENT THIS WAY. USE (Entity):addSpriteComponent(key, path)
    * =========================================
    */
    lua.new_usertype<SpriteComponent>("SpriteComponent",
        sol::meta_function::construct,
        sol::factories(
            [this](std::string key, std::string filepath) {
                std::cout << "Please use (entity):addSpriteComponent(key, path) instead of SpriteComponent.new()!" << std::endl;
                std::shared_ptr<SpriteComponent> spriteComponent = std::make_shared<SpriteComponent>(key);

                return spriteComponent;
            }
        ),
        "x", sol::property(&SpriteComponent::getX, &SpriteComponent::setX),
                "y", sol::property(&SpriteComponent::getY, &SpriteComponent::setY),
                "rotation", sol::property(&SpriteComponent::getRotation, &SpriteComponent::setRotation)
                );

    lua["SpriteComponent"]["getSize"] = [this, &lua](SpriteComponent& component)
    {
        return lua.create_table_with(
            "width", component.getSize().x,
            "height", component.getSize().y
        );
    };

    lua["SpriteComponent"]["getOrigin"] = [this, &lua](SpriteComponent& component)
    {
        return lua.create_table_with(
            "x", component.getOrigin().x,
            "y", component.getOrigin().y
        );
    };

    lua["SpriteComponent"]["setOrigin"] = [this](SpriteComponent& component, float x, float y)
    {
        component.setOrigin(sf::Vector2f(x, y));
    };

    /*
    * ANIMATIONS.
    */

    lua["SpriteComponent"]["setFrameDimensions"] = [this](SpriteComponent& component, int width, int height)
    {
        component.setFrameDimensions(width, height);
    };

    lua["SpriteComponent"]["setRow"] = [this](SpriteComponent& component, int row)
    {
        component.setRow(row);
    };

    lua["SpriteComponent"]["setFrame"] = [this](SpriteComponent& component, int frame)
    {
        component.setFrame(frame);
    };

    lua["SpriteComponent"]["setMinFrame"] = [this](SpriteComponent& component, int minFrame)
    {
        component.setMinFrame(minFrame);
    };

    lua["SpriteComponent"]["setMaxFrame"] = [this](SpriteComponent& component, int maxFrame)
    {
        component.setMaxFrame(maxFrame);
    };

    lua["SpriteComponent"]["setFrameDuration"] = [this](SpriteComponent& component, float duration)
    {
        component.setFrameDuration(duration);
    };

    lua["SpriteComponent"]["playAnimation"] = [this](SpriteComponent& component)
    {
        component.playAnimation();
    };

    lua["SpriteComponent"]["pauseAnimation"] = [this](SpriteComponent& component)
    {
        component.pauseAnimation();
    };

    lua["SpriteComponent"]["setMaxIterations"] = [this](SpriteComponent& component, int maxIterations)
    {
        component.setMaxAnimationIterations(maxIterations);
    };

    lua["SpriteComponent"]["setAnimationFunction"] = [this](SpriteComponent& component, int animationFunction)
    {
        component.setAnimationFunction(static_cast<SpriteAnimationFunction>(animationFunction));
    };

    lua["spark"]["animation"] = lua.create_table_with(
        "forward", 0,
        "boomerang", 1,
        "reverse", 2
    );
}