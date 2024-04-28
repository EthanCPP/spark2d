#include "LuaApi.h"

LuaApi::LuaApi(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals)
{
    mSceneManager = sceneManager;
    mWindow = window;
    mGlobals = globals;

    lua.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table, sol::lib::coroutine);
}

LuaApi::~LuaApi()
{
}

void LuaApi::setup()
{
    mLuaApiMessage = LuaApiMessage::NONE;

    lua["spark"] = lua.create_table();

    setupScene();
    setupEntity();
    setupSpriteComponent();
    setupKeyboard();
    setupUtils();
    setupTime();

    //setupSandbox();
}

void LuaApi::setup(std::shared_ptr<GameEntity> entity)
{
    mEntity = entity;

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

void LuaApi::setupScene()
{
    /*
    * =========================================
    * Create a new scene object
    * =========================================
    */

    // -- local scene = Scene.new("key")
    lua.new_usertype<Scene>("Scene",
        sol::meta_function::construct,
        sol::factories(
            [this](std::string key) {
                std::shared_ptr<Scene> scene = std::make_shared<Scene>(key);
                mSceneManager->addScene(key, scene);

                return scene;
            }
        )
    );

    /*
    * =========================================
    * Start the scene
    * =========================================
    */

    // -- scene:start()
    lua["Scene"]["start"] = [this](Scene& scene)
    {
        mSceneManager->setScene(scene.key);
    };

    /*
    * =========================================
    * Create a new game entity object directly on the scene
    * =========================================
    */

    // -- scene:createEntity("key")
    lua["Scene"]["createEntity"] = [this](Scene& scene, std::string key) {
        std::shared_ptr<GameEntity> entity = std::make_shared<GameEntity>();
        entity->sceneKey = scene.key;
        entity->key = key;

        scene.entityManager->addEntity(key, entity);

        return entity;
    };

    lua["Scene"]["getEntity"] = [this](Scene& scene, std::string key) {
        return scene.entityManager->getEntity(key);
    };
}

void LuaApi::setupEntity()
{
    lua.new_usertype<GameEntityProperties>("GameEntityProperties",
        sol::meta_function::new_index, &GameEntityProperties::setPropertyLua,
        sol::meta_function::index, &GameEntityProperties::getPropertyLua
    );

    /*
    * =========================================
    * Create a new game entity object on the specified scene
    * =========================================
    */

    // -- local entity = Entity.new("sceneKey", "key")
    lua.new_usertype<GameEntity>("Entity",
        sol::meta_function::construct,
        sol::factories(
            [this](std::string sceneKey, std::string key) {
                std::shared_ptr<GameEntity> entity = std::make_shared<GameEntity>();
                entity->sceneKey = sceneKey;
                entity->key = key;

                mSceneManager->getScene(sceneKey)->entityManager->addEntity(key, entity);

                return entity;
            }
        ),
        "x", sol::property(&GameEntity::getX, &GameEntity::setX),
        "y", sol::property(&GameEntity::getY, &GameEntity::setY),
        "props", sol::property(&GameEntity::getDynamicProps)
    );


    /*
    * =========================================
    * Convert the sprite component to a shared pointer, add it to the desired entity
    * =========================================
    */

    // -- entity:addSpriteComponent(sprite)
    lua["Entity"]["addSpriteComponent"] = [this](GameEntity& entity, std::string key, std::string filepath)
    {
        std::shared_ptr<SpriteComponent> spriteComponent = std::make_shared<SpriteComponent>(key, filepath);
        mSceneManager->getScene(entity.sceneKey)->entityManager->getEntity(entity.key)->addSpriteComponent(spriteComponent);

        return spriteComponent;
    };

    // -- entity:getSpriteComponent(key)
    lua["Entity"]["getSpriteComponent"] = [this](GameEntity& entity, std::string key)
    {
        // todo is all this really necessary?
        /*std::shared_ptr<IComponent> component = mSceneManager->getScene(entity.sceneKey)->entityManager->getEntity(entity.key)->getComponent(key);
        std::shared_ptr<SpriteComponent> spriteComponent = std::dynamic_pointer_cast<SpriteComponent>(component);
        return spriteComponent;*/

        return std::dynamic_pointer_cast<SpriteComponent>(entity.getComponent(key));
    };

    lua["Entity"]["addScript"] = [this](GameEntity& entity, std::string path)
    {
        // todo implement this better
        mLuaApiMessage = LuaApiMessage::PUSH_LUA_STATE;

        LuaApiPushState pushState;
        pushState.entity = mSceneManager->getScene(entity.sceneKey)->entityManager->getEntity(entity.key);
        pushState.file = path;

        mLuaApiPushStates.push_back(pushState);
    };

    lua["spark"]["entity"] = [this](sol::table spark) {
        return mEntity;
    };
}

void LuaApi::setupSpriteComponent()
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
                std::shared_ptr<SpriteComponent> spriteComponent = std::make_shared<SpriteComponent>(key, filepath);

                return spriteComponent;
            }
        ),
        "x", sol::property(&SpriteComponent::getX, &SpriteComponent::setX),
        "y", sol::property(&SpriteComponent::getY, &SpriteComponent::setY)
    );

}

void LuaApi::setupSandbox()
{
}

void LuaApi::update(std::vector<sf::Keyboard::Scancode> keysDown, float dt)
{
    mKeysDown = keysDown;
    mDt = dt;

    lua["spark"]["time"]["delta"] = mDt;
    lua["spark"]["update"]();

    /*
    Go through all coroutines that are currently waiting and determine if they have expired
    TODO: find an efficient way of removing these from the vector that doesn't cause memory issues
    */
    if (!mCoroutineWaiters.empty())
    {
        for (auto& waiter : mCoroutineWaiters)
        {
            if (waiter.complete)
                continue;

            if (waiter.clock.getElapsedTime().asSeconds() >= waiter.seconds)
            {
                waiter.complete = true;

                sol::function co = mCoroutineWrap(waiter.coroutine);
                co();
            }
        }
    }
}

void LuaApi::keyDown(sf::Keyboard::Scancode code)
{
    lua["spark"]["keypressed"](sol::nil, code);
}

void LuaApi::keyUp(sf::Keyboard::Scancode code)
{
    lua["spark"]["keyreleased"](sol::nil, code);
}

void LuaApi::setupKeyboard()
{
    /*
    * =========================================
    * Instantiate the keyboard table of utilities
    * =========================================
    */
    lua["spark"]["keyboard"] = lua.create_table();


    /*
    * =========================================
    * Key code bindings
    * TODO put this somewhere nicer
    * =========================================
    */

    // -- spark.keyboard.code.a 
    lua["spark"]["keyboard"]["code"] = lua.create_table_with(
        "a", 0,
        "b", 1,
        "c", 2,
        "d", 3,
        "e", 4,
        "f", 5,
        "g", 6,
        "h", 7,
        "i", 8,
        "j", 9,
        "k", 10,
        "l", 11,
        "m", 12,
        "n", 13,
        "o", 14,
        "p", 15,
        "q", 16,
        "r", 17,
        "s", 18,
        "t", 19,
        "u", 20,
        "v", 21,
        "w", 22,
        "x", 23,
        "y", 24,
        "z", 25,
        "num1", 26,
        "num2", 27,
        "num3", 28,
        "num4", 29,
        "num5", 30,
        "num6", 31,
        "num7", 32,
        "num8", 33,
        "num9", 34,
        "num0", 35,
        "enter", 36,
        "escape", 37,
        "backspace", 38,
        "tab", 39,
        "space", 40,
        "hyphen", 41,
        "equal", 42,
        "lbracket", 43,
        "rbracket", 44,
        "backslash", 45,
        "semicolon", 46,
        "apostrophe", 47,
        "grave", 48,
        "comma", 49,
        "period", 50,
        "slash", 51,
        "f1", 52,
        "f2", 53,
        "f3", 54,
        "f4", 55,
        "f5", 56,
        "f6", 57,
        "f7", 58,
        "f8", 59,
        "f9", 60,
        "f10", 61,
        "f11", 62,
        "f12", 63,
        "f13", 64,
        "f14", 65,
        "f15", 66,
        "f16", 67,
        "f17", 68,
        "f18", 69,
        "f19", 70,
        "f20", 71,
        "f21", 72,
        "f22", 73,
        "f23", 74,
        "f24", 75,
        "capslock", 76,
        "printscreen", 77,
        "scrolllock", 78,
        "pause", 79,
        "insert", 80,
        "home", 81,
        "pageup", 82,
        "delete", 83,
        "end", 84,
        "pagedown", 85,
        "right", 86,
        "left", 87,
        "down", 88,
        "up", 89,
        "numlock", 90,
        "numpaddivide", 91,
        "numpadmultiply", 92,
        "numpadminus", 93,
        "numpadplus", 94,
        "numpadequal", 95,
        "numpadenter", 96,
        "numpaddecimal", 97,
        "numpad1", 98,
        "numpad2", 99,
        "numpad3", 100,
        "numpad4", 101,
        "numpad5", 102,
        "numpad6", 103,
        "numpad7", 104,
        "numpad8", 105,
        "numpad9", 106,
        "numpad0", 107,
        "lcontrol", 127,
        "lshift", 128,
        "lalt", 129,
        "lsystem", 130,
        "rcontrol", 131,
        "rshift", 132,
        "ralt", 133,
        "rsystem", 134
    );


    /*
    * =========================================
    * Active check to see if a particular key is being pressed
    * =========================================
    */

    // -- spark.keyboard:down(3)
    lua["spark"]["keyboard"]["down"] = [this](sol::table keyboard, int code) {
        return (std::find(mKeysDown.begin(), mKeysDown.end(), code) != mKeysDown.end());
    };
}

LuaApiMessage LuaApi::getMessage()
{
    LuaApiMessage message = mLuaApiMessage;
    mLuaApiMessage = LuaApiMessage::NONE;

    return message;
}

std::vector<LuaApiPushState> LuaApi::getPushStates()
{
    std::vector<LuaApiPushState> pushStates = mLuaApiPushStates;
    mLuaApiPushStates.clear();
    
    return pushStates;
}

void LuaApi::setupUtils()
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
}

void LuaApi::setupTime()
{
    lua["spark"]["time"] = lua.create_table_with(
        "delta", 0.01f
    );

    lua["spark"]["time"]["wait"] = sol::yielding([this](sol::table time, sol::function f, float seconds) {
        LuaApiWaiter waiter;
        waiter.coroutine = f;
        waiter.seconds = seconds;
        waiter.clock.restart();

        mCoroutineWaiters.push_back(waiter);
    });

    mCoroutineWrap = lua["coroutine"]["wrap"];
    lua["coroutine"]["wrap"] = [this](sol::function f, sol::this_state ts) {
        sol::function co = mCoroutineWrap(f);
        return co;
    };
}