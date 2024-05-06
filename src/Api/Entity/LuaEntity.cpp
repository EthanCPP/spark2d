#include "LuaEntity.h"


LuaEntity::LuaEntity(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals)
{
    mSceneManager = sceneManager;
    mWindow = window;
    mGlobals = globals;
}

LuaEntity::~LuaEntity()
{

}

void LuaEntity::setup(sol::state& lua)
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
                "solid", sol::property(&GameEntity::getSolid, &GameEntity::setSolid),
                "zindex", sol::property(&GameEntity::getZIndex, &GameEntity::setZIndex)
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


    /*
    * =========================================
    * Convert the sound component to a shared pointer, add it to the desired entity
    * =========================================
    */

    // -- entity:addSoundComponent(text)
    lua["Entity"]["addSoundComponent"] = [this](GameEntity& entity, std::string key, std::string filepath)
    {
        std::shared_ptr<SoundComponent> soundComponent = std::make_shared<SoundComponent>(key);
        mSceneManager->getScene(entity.sceneKey)->entityManager->getEntity(entity.key)->addSoundComponent(soundComponent);
        soundComponent->loadSound(filepath);

        return soundComponent;
    };

    // -- entity:getSoundComponent(key)
    lua["Entity"]["getSoundComponent"] = [this](GameEntity& entity, std::string key)
    {
        return std::dynamic_pointer_cast<SoundComponent>(entity.getComponent(key));
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

    lua["Entity"]["getColliders"] = [this, &lua](GameEntity& entity)
    {
        sol::table table = lua.create_table();

        for (auto& collider : entity.mColliders)
        {
            table[collider->key] = collider;
        }

        return table;
    };
}

void LuaEntity::setup(sol::state& lua, std::shared_ptr<GameEntity> entity)
{
    mEntity = entity;

    this->setup(lua);
}

LuaApiMessage LuaEntity::getMessage()
{
    LuaApiMessage message = mLuaApiMessage;
    mLuaApiMessage = LuaApiMessage::NONE;

    return message;
}

std::vector<LuaApiPushState> LuaEntity::getPushStates()
{
    std::vector<LuaApiPushState> pushStates = mLuaApiPushStates;
    mLuaApiPushStates.clear();

    return pushStates;
}