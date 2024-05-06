#include "LuaApi.h"

LuaApi::LuaApi(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals)
{
    mSceneManager = sceneManager;
    mWindow = window;
    mGlobals = globals;

    this->inactive = false;

    lua.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table, sol::lib::coroutine, sol::lib::package);

    mLuaScene = new LuaScene(mSceneManager, mWindow, mGlobals);
    mLuaEntity = new LuaEntity(mSceneManager, mWindow, mGlobals);
    mLuaSpriteComponent = new LuaSpriteComponent(mSceneManager, mWindow, mGlobals);
    mLuaTextComponent = new LuaTextComponent(mSceneManager, mWindow, mGlobals);
    mLuaCircleComponent = new LuaCircleComponent(mSceneManager, mWindow, mGlobals);
    mLuaRectangleComponent = new LuaRectangleComponent(mSceneManager, mWindow, mGlobals);
    mLuaSoundComponent = new LuaSoundComponent(mSceneManager, mWindow, mGlobals);
    mLuaKeyboard = new LuaKeyboard(mSceneManager, mWindow, mGlobals);
    mLuaMouse = new LuaMouse(mSceneManager, mWindow, mGlobals);
    mLuaUtils = new LuaUtils(mSceneManager, mWindow, mGlobals);
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

    mLuaSpriteComponent->setup(lua);
    mLuaTextComponent->setup(lua);
    mLuaCircleComponent->setup(lua);
    mLuaRectangleComponent->setup(lua);
    mLuaSoundComponent->setup(lua);
    mLuaKeyboard->setup(lua);
    mLuaMouse->setup(lua);
    mLuaUtils->setup(lua);

    // setupScene();
    // setupEntity();
    // setupSpriteComponent();
    // setupTextComponent();
    //setupCircleComponent();
    //setupRectangleComponent();
    //setupSoundComponent();
    //setupKeyboard();
    //setupMouse();
    //setupUtils();
    setupTime();
    setupGuiEntity();

    // todo somewhere else for this?
    lua["Entity"]["destroy"] = [this](GameEntity& entity)
    {
        entitiesToDestroy.push_back(std::make_shared<GameEntity>(entity));
    };

    //setupSandbox();
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

void LuaApi::setupScene()
{
}

void LuaApi::setupEntity()
{
}

void LuaApi::setupSpriteComponent()
{
}

void LuaApi::setupTextComponent()
{
}

void LuaApi::setupCircleComponent()
{
}

void LuaApi::setupRectangleComponent()
{
}

void LuaApi::setupSoundComponent()
{
}

void LuaApi::setupSandbox()
{
}

void LuaApi::update(std::vector<sf::Keyboard::Scancode> keysDown, float dt)
{
    mLuaKeyboard->keysDown = keysDown;
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

    mSceneManager->getCurrentScene()->guiManager->mouseX = mMouseX;
    mSceneManager->getCurrentScene()->guiManager->mouseY = mMouseY;
}

void LuaApi::setupKeyboard()
{
}

void LuaApi::setupMouse()
{
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

void LuaApi::setupUtils()
{

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

void LuaApi::setupGuiEntity()
{
    /*
    * =========================================
    * Create a new gui entity object on the specified scene
    * =========================================
    */

    // -- local guiEntity = GuiEntity.new("sceneKey", "key")
    lua.new_usertype<GuiEntity>("GuiEntity",
        sol::meta_function::construct,
        sol::factories(
            [this](std::string sceneKey, std::string key) {
                std::shared_ptr<GuiEntity> entity = std::make_shared<GuiEntity>();
                entity->sceneKey = sceneKey;
                entity->key = key;

                mSceneManager->getScene(sceneKey)->guiManager->addEntity(key, entity);

                return entity;
            }
        ),
        "x", sol::property(&GuiEntity::getX, &GuiEntity::setX),
        "y", sol::property(&GuiEntity::getY, &GuiEntity::setY),
        "rotation", sol::property(&GuiEntity::getRotation, &GuiEntity::setRotation),
        "zindex", sol::property(&GuiEntity::getZIndex, &GuiEntity::setZIndex),
        "debug", sol::property(&GuiEntity::getBoundaryDebug, &GuiEntity::setBoundaryDebug),
        "mouseover", sol::property(&GuiEntity::getMouseOver)
    );


    /*
    * =========================================
    * Convert the sprite component to a shared pointer, add it to the desired entity
    * =========================================
    */

    // -- entity:addSpriteComponent(sprite)
    lua["GuiEntity"]["addSpriteComponent"] = [this](GuiEntity& entity, std::string key, std::string filepath)
    {
        std::shared_ptr<SpriteComponent> spriteComponent = std::make_shared<SpriteComponent>(key);
        mSceneManager->getScene(entity.sceneKey)->guiManager->getEntity(entity.key)->addSpriteComponent(spriteComponent);
        spriteComponent->loadTexture(filepath);

        return spriteComponent;
    };

    // -- entity:getSpriteComponent(key)
    lua["GuiEntity"]["getSpriteComponent"] = [this](GuiEntity& entity, std::string key)
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
    lua["GuiEntity"]["addTextComponent"] = [this](GuiEntity& entity, std::string key, std::string filepath)
    {
        std::shared_ptr<TextComponent> textComponent = std::make_shared<TextComponent>(key);
        mSceneManager->getScene(entity.sceneKey)->guiManager->getEntity(entity.key)->addTextComponent(textComponent);
        textComponent->loadFont(filepath);

        return textComponent;
    };

    // -- entity:getTextComponent(key)
    lua["GuiEntity"]["getTextComponent"] = [this](GuiEntity& entity, std::string key)
    {
        return std::dynamic_pointer_cast<TextComponent>(entity.getComponent(key));
    };


    /*
    * =========================================
    * Convert the circle component to a shared pointer, add it to the desired entity
    * =========================================
    */

    // -- entity:addCircleComponent(text)
    lua["GuiEntity"]["addCircleComponent"] = [this](GuiEntity& entity, std::string key)
    {
        std::shared_ptr<CircleComponent> circleComponent = std::make_shared<CircleComponent>(key);
        mSceneManager->getScene(entity.sceneKey)->guiManager->getEntity(entity.key)->addCircleComponent(circleComponent);

        return circleComponent;
    };

    // -- entity:getCircleComponent(key)
    lua["GuiEntity"]["getCircleComponent"] = [this](GuiEntity& entity, std::string key)
    {
        return std::dynamic_pointer_cast<CircleComponent>(entity.getComponent(key));
    };


    /*
    * =========================================
    * Convert the rectangle component to a shared pointer, add it to the desired entity
    * =========================================
    */

    // -- entity:addRectangleComponent(text)
    lua["GuiEntity"]["addRectangleComponent"] = [this](GuiEntity& entity, std::string key)
    {
        std::shared_ptr<RectangleComponent> rectangleComponent = std::make_shared<RectangleComponent>(key);
        mSceneManager->getScene(entity.sceneKey)->guiManager->getEntity(entity.key)->addRectangleComponent(rectangleComponent);

        return rectangleComponent;
    };

    // -- entity:getRectangleComponent(key)
    lua["GuiEntity"]["getRectangleComponent"] = [this](GuiEntity& entity, std::string key)
    {
        return std::dynamic_pointer_cast<RectangleComponent>(entity.getComponent(key));
    };


    /*
    * =========================================
    * Convert the sound component to a shared pointer, add it to the desired entity
    * =========================================
    */

    // -- entity:addSoundComponent(text)
    lua["GuiEntity"]["addSoundComponent"] = [this](GuiEntity& entity, std::string key, std::string filepath)
    {
        std::shared_ptr<SoundComponent> soundComponent = std::make_shared<SoundComponent>(key);
        mSceneManager->getScene(entity.sceneKey)->guiManager->getEntity(entity.key)->addSoundComponent(soundComponent);
        soundComponent->loadSound(filepath);

        return soundComponent;
    };

    // -- entity:getSoundComponent(key)
    lua["GuiEntity"]["getSoundComponent"] = [this](GuiEntity& entity, std::string key)
    {
        return std::dynamic_pointer_cast<SoundComponent>(entity.getComponent(key));
    };

    lua["GuiEntity"]["setBoundarySize"] = [this](GuiEntity& entity, float width, float height)
    {
        entity.setBoundarySize(width, height);
    };

    lua["GuiEntity"]["setBoundaryOffset"] = [this](GuiEntity& entity, float x, float y)
    {
        entity.setBoundaryOffset(x, y);
    };
}