#include "LuaGuiEntity.h"


LuaGuiEntity::LuaGuiEntity(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals)
{
    mSceneManager = sceneManager;
    mWindow = window;
    mGlobals = globals;
}

LuaGuiEntity::~LuaGuiEntity()
{

}

void LuaGuiEntity::setup(sol::state& lua)
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