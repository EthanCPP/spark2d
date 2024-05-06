#include "LuaUtils.h"


LuaUtils::LuaUtils(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals)
{
    mSceneManager = sceneManager;
    mWindow = window;
    mGlobals = globals;
}

LuaUtils::~LuaUtils()
{

}

void LuaUtils::setup(sol::state& lua)
{
    /*
    * =========================================
    * Allow the dev to change the window title, width, height
    * =========================================
    */

    lua["spark"]["setWindowSize"] = [this](sol::table spark, float width, float height) {
        mGlobals->windowWidth = width;
        mGlobals->windowHeight = height;
    };

    lua["spark"]["setWindowTitle"] = [this](sol::table spark, std::string title) {
        mGlobals->windowTitle = title;
    };

    lua["spark"]["setFramerateLimit"] = [this](sol::table spark, int framerate) {
        mGlobals->framerateLimit = framerate;
    };

    lua["spark"]["regenerateWindow"] = [this](sol::table spark) {
        mWindow->create(sf::VideoMode(mGlobals->windowWidth, mGlobals->windowHeight), mGlobals->windowTitle);
        mWindow->setFramerateLimit(mGlobals->framerateLimit);
    };

    /*
    * =========================================
    * Allow the dev to change the physics properties
    * =========================================
    */

    lua["spark"]["setGravity"] = [this](sol::table spark, float x, float y) {
        mGlobals->gravity = sf::Vector2f(x, y);
    };

    /*
    * =========================================
    * Music
    * =========================================
    */
    lua["spark"]["music"] = lua.create_table();
    lua["spark"]["music"]["load"] = [this](sol::table spark, std::string key, std::string file)
    {
        mSceneManager->addMusic(key, file);
    };

    lua["spark"]["music"]["play"] = [this](sol::table spark, std::string key)
    {
        mSceneManager->playMusic(key);
    };

    lua["spark"]["music"]["pause"] = [this](sol::table spark, std::string key)
    {
        mSceneManager->pauseMusic(key);
    };
    lua["spark"]["music"]["stop"] = [this](sol::table spark, std::string key)
    {
        mSceneManager->stopMusic(key);
    };
}