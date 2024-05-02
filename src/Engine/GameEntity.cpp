#include "GameEntity.h"

GameEntity::GameEntity()
{
}

GameEntity::~GameEntity()
{
    // Delete all lua states associated with this entity.
    mLuaStates.clear();
}

void GameEntity::init(ResourceManager* resourceManager)
{
    mResourceManager = resourceManager;
}

void GameEntity::addLuaState(std::shared_ptr<LuaApi> luaState)
{
   mLuaStates.push_back(luaState);
}

void GameEntity::addSpriteComponent(std::shared_ptr<SpriteComponent> spriteComponent)
{
    // todo: safer way of keying
    spriteComponent->init(mResourceManager);
    mComponents.insert({ spriteComponent->key, spriteComponent });
}

void GameEntity::addTextComponent(std::shared_ptr<TextComponent> textComponent)
{
    // todo: safer way of keying
    textComponent->init(mResourceManager);
    mComponents.insert({ textComponent->key, textComponent });
}

void GameEntity::addCircleComponent(std::shared_ptr<CircleComponent> circleComponent)
{
    // todo: safer way of keying
    circleComponent->init(mResourceManager);
    mComponents.insert({ circleComponent->key, circleComponent });
}

void GameEntity::addRectangleComponent(std::shared_ptr<RectangleComponent> rectangleComponent)
{
    // todo: safer way of keying
    rectangleComponent->init(mResourceManager);
    mComponents.insert({ rectangleComponent->key, rectangleComponent });
}

std::shared_ptr<IComponent> GameEntity::getComponent(std::string key)
{
    return mComponents[key];
}

void GameEntity::update()
{
    for (auto const& component : mComponents)
    {
        component.second->update(transform);
    }
}

void GameEntity::render(std::shared_ptr<sf::RenderWindow> window)
{
    for (auto const& component : mComponents)
    {
        if (component.second->isDrawable()) {
            component.second->render(window);
        }
    }
}

float GameEntity::getX()
{
    return transform.position.x;
}

float GameEntity::getY()
{
    return transform.position.y;
}

float GameEntity::getRotation()
{
    return transform.rotation.angle;
}

void GameEntity::setX(float x)
{
    transform.position.x = x;
}

void GameEntity::setY(float y)
{
    transform.position.y = y;
}

void GameEntity::setRotation(float angle)
{
    transform.rotation.angle = angle;
}

GameEntityProperties& GameEntity::getDynamicProps()
{
    return mDynamicProps;
}