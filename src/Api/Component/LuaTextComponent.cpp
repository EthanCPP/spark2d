#include "LuaTextComponent.h"


LuaTextComponent::LuaTextComponent(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals)
{
    mSceneManager = sceneManager;
    mWindow = window;
    mGlobals = globals;
}

LuaTextComponent::~LuaTextComponent()
{

}

void LuaTextComponent::setup(sol::state& lua)
{
    /*
    * =========================================
    * Create a new text component object with desired font
    *
    * DON'T CREATE TEXTCOMPONENT THIS WAY. USE (Entity):addTextComponent(key, path)
    * =========================================
    */
    lua.new_usertype<TextComponent>("TextComponent",
        sol::meta_function::construct,
        sol::factories(
            [this](std::string key, std::string filepath) {
                std::cout << "Please use (entity):addTextComponent(key, path) instead of TextComponent.new()!" << std::endl;
                std::shared_ptr<TextComponent> textComponent = std::make_shared<TextComponent>(key);

                return textComponent;
            }
        ),
        "x", sol::property(&TextComponent::getX, &TextComponent::setX),
                "y", sol::property(&TextComponent::getY, &TextComponent::setY),
                "rotation", sol::property(&TextComponent::getRotation, &TextComponent::setRotation)
                );

    lua["TextComponent"]["setText"] = [this](TextComponent& component, std::string text)
    {
        component.setText(text);
    };

    lua["TextComponent"]["setCharacterSize"] = [this](TextComponent& component, int size)
    {
        component.setCharacterSize(size);
    };

    lua["TextComponent"]["setColour"] = [this](TextComponent& component, int r, int g, int b)
    {
        component.setFillColour(r, g, b, 255);
    };

    lua["TextComponent"]["setColourAlpha"] = [this](TextComponent& component, int r, int g, int b, int a)
    {
        component.setFillColour(r, g, b, a);
    };

    lua["TextComponent"]["setBold"] = [this](TextComponent& component, bool bold)
    {
        component.setBold(bold);
    };

    lua["TextComponent"]["setUnderlined"] = [this](TextComponent& component, bool underlined)
    {
        component.setUnderlined(underlined);
    };

    lua["TextComponent"]["setItalic"] = [this](TextComponent& component, bool italic)
    {
        component.setItalic(italic);
    };
}