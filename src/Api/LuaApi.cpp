#include "LuaApi.h"

LuaApi::LuaApi(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals)
{
    mSceneManager = sceneManager;
    mWindow = window;
    mGlobals = globals;

    this->inactive = false;

    lua.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table, sol::lib::coroutine, sol::lib::package);

    // set package.path
    const std::string package_path = lua["package"]["path"];
    const std::string current_path = std::string(std::filesystem::current_path().u8string());
    const std::string spark_search_paths[2] = {
        "/data/scripts/?/?.lua",
        "/data/scripts/?.lua"
    };

    if (globals->isDebug)
    {
        lua["package"]["path"] = package_path + ";" +
            current_path + "/x64/Debug" + spark_search_paths[0] + ";" +
            current_path + "/x64/Debug" + spark_search_paths[1];
    }
    else
    {
        lua["package"]["path"] = package_path + ";" +
            current_path + spark_search_paths[0] + ";" +
            current_path + spark_search_paths[1];
    }

    mLuaScene = new LuaScene(mSceneManager, mWindow, mGlobals);
    mLuaEntity = new LuaEntity(mSceneManager, mWindow, mGlobals);
    mLuaGuiEntity = new LuaGuiEntity(mSceneManager, mWindow, mGlobals);
    mLuaSpriteComponent = new LuaSpriteComponent(mSceneManager, mWindow, mGlobals);
    mLuaTextComponent = new LuaTextComponent(mSceneManager, mWindow, mGlobals);
    mLuaCircleComponent = new LuaCircleComponent(mSceneManager, mWindow, mGlobals);
    mLuaRectangleComponent = new LuaRectangleComponent(mSceneManager, mWindow, mGlobals);
    mLuaSoundComponent = new LuaSoundComponent(mSceneManager, mWindow, mGlobals);
    mLuaKeyboard = new LuaKeyboard(mSceneManager, mWindow, mGlobals);
    mLuaMouse = new LuaMouse(mSceneManager, mWindow, mGlobals);
    mLuaUtils = new LuaUtils(mSceneManager, mWindow, mGlobals);
    mLuaTime = new LuaTime(mSceneManager, mWindow, mGlobals);
}

LuaApi::~LuaApi()
{
}

void LuaApi::setup()
{
    lua["spark"] = lua.create_table();

    mLuaScene->setup(lua);

    if (mEntityAttached)
        mLuaEntity->setup(lua, mEntity);
    else
        mLuaEntity->setup(lua);

    mLuaGuiEntity->setup(lua);
    mLuaSpriteComponent->setup(lua);
    mLuaTextComponent->setup(lua);
    mLuaCircleComponent->setup(lua);
    mLuaRectangleComponent->setup(lua);
    mLuaSoundComponent->setup(lua);
    mLuaKeyboard->setup(lua);
    mLuaMouse->setup(lua);
    mLuaUtils->setup(lua);
    mLuaTime->setup(lua);


    // todo somewhere else for this?
    lua["Entity"]["destroy"] = [this](GameEntity& entity)
    {
        entitiesToDestroy.push_back(std::make_shared<GameEntity>(entity));
    };
}

void LuaApi::setup(std::shared_ptr<GameEntity> entity)
{
    mEntity = entity;
    mEntityAttached = true;

    this->setup();
}

bool LuaApi::init(std::string script)
{
    try
    {
        lua.safe_script_file(script);
    }
    catch (const sol::error& e)
    {
        std::cout << "Could not read entrypoint" << std::endl;
        std::cout << std::string(e.what()) << std::endl;
        return false;
    }

    return true;
}

void LuaApi::update(std::vector<sf::Keyboard::Scancode> keysDown, float dt)
{
    mLuaKeyboard->keysDown = keysDown;
    mDt = dt;

    lua["spark"]["time"]["delta"] = mDt;
    lua["spark"]["update"]();

    mLuaTime->update(dt);
}

void LuaApi::keyDown(sf::Keyboard::Scancode code)
{
    mLuaKeyboard->keyDown(lua, code);
}

void LuaApi::keyUp(sf::Keyboard::Scancode code)
{
    mLuaKeyboard->keyUp(lua, code);
}

void LuaApi::mouseDown(sf::Mouse::Button button)
{
    mLuaMouse->mouseDown(lua, button);
}

void LuaApi::mouseUp(sf::Mouse::Button button)
{
    mLuaMouse->mouseUp(lua, button);
}

void LuaApi::mouseWheelMoved(int direction)
{
    mLuaMouse->mouseWheelMoved(lua, direction);
}

void LuaApi::mouseMoved(float x, float y)
{
    mLuaMouse->mouseMoved(lua, x, y);
}

LuaApiMessage LuaApi::getMessage()
{
    LuaApiMessage entityMessage = mLuaEntity->getMessage();

    return entityMessage;
}

std::vector<LuaApiPushState> LuaApi::getPushStates()
{
    std::vector<LuaApiPushState> entityPushStates = mLuaEntity->getPushStates();

    return entityPushStates;
}