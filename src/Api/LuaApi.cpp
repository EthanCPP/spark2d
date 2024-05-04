#include "LuaApi.h"

LuaApi::LuaApi(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals)
{
    mSceneManager = sceneManager;
    mWindow = window;
    mGlobals = globals;

    this->inactive = false;

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
    setupTextComponent();
    setupCircleComponent();
    setupRectangleComponent();
    setupKeyboard();
    setupMouse();
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

    lua["spark"]["getScene"] = [this](sol::table spark, std::string key) {
        return mSceneManager->getScene(key);
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
        "rotation", sol::property(&GameEntity::getRotation, &GameEntity::setRotation),
        "props", sol::property(&GameEntity::getDynamicProps),
        "static", sol::property(&GameEntity::getStatic, &GameEntity::setStatic),
        "vx", sol::property(&GameEntity::getXVelocity, &GameEntity::setXVelocity),
        "vy", sol::property(&GameEntity::getYVelocity, &GameEntity::setYVelocity),
        "debug", sol::property(&GameEntity::getColliderDebug, &GameEntity::setColliderDebug),
        "solid", sol::property(&GameEntity::getSolid, &GameEntity::setSolid)
    );


    /*
    * =========================================
    * Convert the sprite component to a shared pointer, add it to the desired entity
    * =========================================
    */

    // -- entity:addSpriteComponent(sprite)
    lua["Entity"]["addSpriteComponent"] = [this](GameEntity& entity, std::string key, std::string filepath)
    {
        std::shared_ptr<SpriteComponent> spriteComponent = std::make_shared<SpriteComponent>(key);
        mSceneManager->getScene(entity.sceneKey)->entityManager->getEntity(entity.key)->addSpriteComponent(spriteComponent);
        spriteComponent->loadTexture(filepath);

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


    /*
    * =========================================
    * Convert the text component to a shared pointer, add it to the desired entity
    * =========================================
    */

    // -- entity:addTextComponent(text)
    lua["Entity"]["addTextComponent"] = [this](GameEntity& entity, std::string key, std::string filepath)
    {
        std::shared_ptr<TextComponent> textComponent = std::make_shared<TextComponent>(key);
        mSceneManager->getScene(entity.sceneKey)->entityManager->getEntity(entity.key)->addTextComponent(textComponent);
        textComponent->loadFont(filepath);

        return textComponent;
    };

    // -- entity:getTextComponent(key)
    lua["Entity"]["getTextComponent"] = [this](GameEntity& entity, std::string key)
    {
        return std::dynamic_pointer_cast<TextComponent>(entity.getComponent(key));
    };


    /*
    * =========================================
    * Convert the circle component to a shared pointer, add it to the desired entity
    * =========================================
    */

    // -- entity:addCircleComponent(text)
    lua["Entity"]["addCircleComponent"] = [this](GameEntity& entity, std::string key)
    {
        std::shared_ptr<CircleComponent> circleComponent = std::make_shared<CircleComponent>(key);
        mSceneManager->getScene(entity.sceneKey)->entityManager->getEntity(entity.key)->addCircleComponent(circleComponent);

        return circleComponent;
    };

    // -- entity:getCircleComponent(key)
    lua["Entity"]["getCircleComponent"] = [this](GameEntity& entity, std::string key)
    {
        return std::dynamic_pointer_cast<CircleComponent>(entity.getComponent(key));
    };


    /*
    * =========================================
    * Convert the rectangle component to a shared pointer, add it to the desired entity
    * =========================================
    */

    // -- entity:addRectangleComponent(text)
    lua["Entity"]["addRectangleComponent"] = [this](GameEntity& entity, std::string key)
    {
        std::shared_ptr<RectangleComponent> rectangleComponent = std::make_shared<RectangleComponent>(key);
        mSceneManager->getScene(entity.sceneKey)->entityManager->getEntity(entity.key)->addRectangleComponent(rectangleComponent);

        return rectangleComponent;
    };

    // -- entity:getRectangleComponent(key)
    lua["Entity"]["getRectangleComponent"] = [this](GameEntity& entity, std::string key)
    {
        return std::dynamic_pointer_cast<RectangleComponent>(entity.getComponent(key));
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

    lua["Entity"]["destroy"] = [this](GameEntity& entity)
    {
        entitiesToDestroy.push_back(std::make_shared<GameEntity>(entity));
    };

    lua["Entity"]["applyVelocity"] = [this](GameEntity& entity, float x, float y)
    {
        entity.applyVelocity(sf::Vector2f(x, y));
    };

    lua["Entity"]["setColliderSize"] = [this](GameEntity& entity, float width, float height)
    {
        entity.setColliderSize(width, height);
    };

    lua["Entity"]["setColliderOffset"] = [this](GameEntity& entity, float x, float y)
    {
        entity.setColliderOffset(x, y);
    };

    lua["Entity"]["checkCollision"] = [this](GameEntity& entity, GameEntity& otherEntity)
    {
        return entity.checkCollision(std::make_shared<GameEntity>(otherEntity));
    };

    lua["Entity"]["checkCollisionKey"] = [this](GameEntity& entity, std::string otherEntityKey)
    {
        return entity.checkCollision(mSceneManager->getCurrentScene()->entityManager->getEntity(otherEntityKey));
    };

    lua["Entity"]["getColliders"] = [this](GameEntity& entity)
    {
        sol::table table = lua.create_table();

        for (auto& collider : entity.mColliders)
        {
            table[collider->key] = collider;
        }

        return table;
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
                std::cout << "Please use (entity):addSpriteComponent(key, path) instead of SpriteComponent.new()!" << std::endl;
                std::shared_ptr<SpriteComponent> spriteComponent = std::make_shared<SpriteComponent>(key);

                return spriteComponent;
            }
        ),
        "x", sol::property(&SpriteComponent::getX, &SpriteComponent::setX),
        "y", sol::property(&SpriteComponent::getY, &SpriteComponent::setY),
        "rotation", sol::property(&SpriteComponent::getRotation, &SpriteComponent::setRotation)
    );

}

void LuaApi::setupTextComponent()
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

void LuaApi::setupCircleComponent()
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

void LuaApi::setupRectangleComponent()
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

void LuaApi::mouseDown(sf::Mouse::Button button)
{
    lua["spark"]["mousedown"](sol::nil, button);
}

void LuaApi::mouseUp(sf::Mouse::Button button)
{
    lua["spark"]["mouseup"](sol::nil, button);
}

void LuaApi::mouseWheelMoved(int direction)
{
    lua["spark"]["mousewheelmoved"](sol::nil, direction);
}

void LuaApi::mouseMoved(float x, float y)
{
    mMouseX = x;
    mMouseY = y;

    lua["spark"]["mousemoved"](sol::nil, x, y);

    lua["spark"]["mouse"]["x"] = mMouseX;
    lua["spark"]["mouse"]["y"] = mMouseY;
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

void LuaApi::setupMouse()
{
    /*
    * =========================================
    * Instantiate the mouse table of utilities
    * =========================================
    */
    lua["spark"]["mouse"] = lua.create_table_with(
        "x", 0,
        "y", 0
    );


    /*
    * =========================================
    * Mouse code bindings
    * TODO put this somewhere nicer
    * =========================================
    */

    // -- spark.mouse.button.left 
    lua["spark"]["mouse"]["button"] = lua.create_table_with(
        "left", 0,
        "right", 1,
        "middle", 2,
        "x1", 3,
        "x2", 4
    );

    lua["spark"]["mouse"]["wheel"] = lua.create_table_with(
        "up", 1,
        "down", -1
    );
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

    /*
    * =========================================
    * Allow the dev to change the physics properties
    * =========================================
    */

    lua["spark"]["setGravity"] = [this](sol::table spark, float x, float y) {
        mGlobals->gravity = sf::Vector2f(x, y);
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