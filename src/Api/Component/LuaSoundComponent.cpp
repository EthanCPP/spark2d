#include "LuaSoundComponent.h"


LuaSoundComponent::LuaSoundComponent(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals)
{
    mSceneManager = sceneManager;
    mWindow = window;
    mGlobals = globals;
}

LuaSoundComponent::~LuaSoundComponent()
{

}

void LuaSoundComponent::setup(sol::state& lua)
{
    /*
    * =========================================
    * Create a new sound component object
    *
    * DON'T CREATE COMPONENT THIS WAY. USE (Entity):addSoundComponent(key)
    * =========================================
    */
    lua.new_usertype<SoundComponent>("SoundComponent",
        sol::meta_function::construct,
        sol::factories(
            [this](std::string key) {
                std::cout << "Please use (entity):addSoundComponent(key, path) instead of SoundComponent.new()!" << std::endl;
                std::shared_ptr<SoundComponent> soundComponent = std::make_shared<SoundComponent>(key);

                return soundComponent;
            }
        ),
        "volume", sol::property(&SoundComponent::getVolume, &SoundComponent::setVolume)
                );

    lua["SoundComponent"]["play"] = [this](SoundComponent& component)
    {
        component.play();
    };
}