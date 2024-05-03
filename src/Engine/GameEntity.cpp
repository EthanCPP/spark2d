#include "GameEntity.h"

GameEntity::GameEntity()
{
}

GameEntity::~GameEntity()
{
    // Delete all lua states associated with this entity.
    mLuaStates.clear();
}

void GameEntity::init(ResourceManager* resourceManager, std::shared_ptr<SparkGlobals> sparkGlobals)
{
    mResourceManager = resourceManager;
    mSparkGlobals = sparkGlobals;

    mStatic = false;
    mVelocity = sf::Vector2f(0.f, 0.f);
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

void GameEntity::update(float dt)
{
    if (!mStatic)
    {
        // Gravity (temp)
        mVelocity += sf::Vector2f(mSparkGlobals->gravity.x * dt, mSparkGlobals->gravity.y * dt);
        transform.position.x += mVelocity.x;
        transform.position.y += mVelocity.y;
    }

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
    return transform.rotation;
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
    transform.rotation = angle;
}

bool GameEntity::getStatic()
{
    return mStatic;
}

void GameEntity::setStatic(bool bStatic)
{
    mStatic = bStatic;
    mVelocity = sf::Vector2f(0.0f, 0.0f);
}

void GameEntity::applyVelocity(sf::Vector2f velocity)
{
    mVelocity += velocity;
}

GameEntityProperties& GameEntity::getDynamicProps()
{
    return mDynamicProps;
}