#include "LuaConfigLoader.h"

LuaConfigLoader::LuaConfigLoader(std::shared_ptr<SparkGlobals> globals, ResourceManager* resourceManager)
{
	mGlobals = globals;
    mResourceManager = resourceManager;

    lua.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table);
}

LuaConfigLoader::~LuaConfigLoader()
{

}

void LuaConfigLoader::setup()
{
    lua["spark"] = lua.create_table();

    setupWindowSettings();
    setupFunctions();
}

bool LuaConfigLoader::init(std::string script)
{
    try
    {
        lua.safe_script_file(script);
    }
    catch (const sol::error& e)
    {
        std::cout << "Could not read config file" << std::endl;
        std::cout << std::string(e.what()) << std::endl;
        return false;
    }

    return true;
}

void LuaConfigLoader::setupWindowSettings()
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

    /*
    * =========================================
    * Allow the dev to change the physics properties
    * =========================================
    */

    lua["spark"]["setGravity"] = [this](sol::table spark, float x, float y) {
        mGlobals->gravity = sf::Vector2f(x, y);
    };
}

void LuaConfigLoader::setupFunctions()
{
    lua["spark"]["preloadTexture"] = [this](sol::table spark, std::string path) {
        mResourceManager->getTexture(path);
    };
}