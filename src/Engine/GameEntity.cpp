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
    mSolid = true;
    mVelocity = sf::Vector2f(0.f, 0.f);
    mCollider = sf::FloatRect(0.f, 0.f, 0.f, 0.f);
    mShowColliderDebug = false;
    mColliderDebug.setOutlineColor(sf::Color(255, 0, 0));
    mColliderDebug.setFillColor(sf::Color(0, 0, 0, 0));
    mColliderDebug.setOutlineThickness(2);
    zIndex = 0;
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

void GameEntity::addSoundComponent(std::shared_ptr<SoundComponent> soundComponent)
{
    // todo: safer way of keying
    soundComponent->init(mResourceManager);
    mComponents.insert({ soundComponent->key, soundComponent });
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
        // todo don't apply gravity if there's something in the way
        mVelocity += sf::Vector2f(mSparkGlobals->gravity.x, mSparkGlobals->gravity.y);

        if (mColliders.size() > 0)
        {
            for (auto& collider : mColliders)
            {
                // Don't debounce if the collider is solid
                if (!collider->mSolid)
                    continue;

                if ((getCollider().top + getCollider().height > collider->getCollider().top) && // The collision is at the bottom of this object
                    (getCollider().left + getCollider().width - 2 > collider->getCollider().left) && // Make sure this object sits between the X coords of the other object
                    (getCollider().left + 2 < collider->getCollider().left + collider->getCollider().width) && 
                    mVelocity.y > 0)
                {
                    // Bottom collisions
                    sf::Vector2f currentPos = sf::Vector2f(getCollider().left, getCollider().top);
                    sf::Vector2f newPos = sf::Vector2f(currentPos.x, collider->getCollider().top - getCollider().height - 1);
                    sf::Vector2f diff = newPos - currentPos;
                    float distance = sqrt(powf(diff.x, 2) + powf(diff.y, 2));

                    if (distance < 2) // Make sure the distance we are snapping to isn't too great
                    {
                        mVelocity.y = 0;
                        transform.position.y = (collider->getCollider().top - getCollider().height - 1) - mCollider.top;
                    }
                }
                else if ((getCollider().top < collider->getCollider().top + collider->getCollider().height) &&
                    (getCollider().left + getCollider().width - 2 > collider->getCollider().left) &&
                    (getCollider().left + 2 < collider->getCollider().left + collider->getCollider().width) &&
                    mVelocity.y < 0)
                {
                    // Top collisions
                    sf::Vector2f currentPos = sf::Vector2f(getCollider().left, getCollider().top);
                    sf::Vector2f newPos = sf::Vector2f(currentPos.x, collider->getCollider().top + collider->getCollider().height + 1);
                    sf::Vector2f diff = newPos - currentPos;
                    float distance = sqrt(powf(diff.x, 2) + powf(diff.y, 2));

                    if (distance < 2)
                    {
                        mVelocity.y = 0;
                        transform.position.y = (collider->getCollider().top + collider->getCollider().height + 1) - mCollider.top;
                    }
                }
                else if ((getCollider().left < collider->getCollider().left + collider->getCollider().width) &&
                    (getCollider().top + getCollider().height - 2 > collider->getCollider().top) &&
                    (getCollider().top + 2 < collider->getCollider().top + collider->getCollider().height) &&
                    mVelocity.x < 0)
                {
                    // Left collisions
                    sf::Vector2f currentPos = sf::Vector2f(getCollider().left, getCollider().top);
                    sf::Vector2f newPos = sf::Vector2f(collider->getCollider().left + collider->getCollider().width + 1, currentPos.y);
                    sf::Vector2f diff = newPos - currentPos;
                    float distance = sqrt(powf(diff.x, 2) + powf(diff.y, 2));

                    if (distance < 2)
                    {
                        mVelocity.x = 0;
                        transform.position.x = (collider->getCollider().left + collider->getCollider().width + 1) - mCollider.left;
                    }
                }
                else if ((getCollider().left + getCollider().width > collider->getCollider().left) &&
                    (getCollider().top + getCollider().height - 2 > collider->getCollider().top) &&
                    (getCollider().top + 2 < collider->getCollider().top + collider->getCollider().height) &&
                    mVelocity.x > 0)
                {
                    // Right collisions
                    sf::Vector2f currentPos = sf::Vector2f(getCollider().left, getCollider().top);
                    sf::Vector2f newPos = sf::Vector2f(collider->getCollider().left - getCollider().width - 1, currentPos.y);
                    sf::Vector2f diff = newPos - currentPos;
                    float distance = sqrt(powf(diff.x, 2) + powf(diff.y, 2));

                    if (distance < 2)
                    {
                        mVelocity.x = 0;
                        transform.position.x = (collider->getCollider().left - getCollider().width - 1) - mCollider.left;
                    }
                }
            }
        }

        transform.position.x += mVelocity.x * dt;
        transform.position.y += mVelocity.y * dt;
    }

    for (auto const& component : mComponents)
    {
        component.second->update(transform);
    }

    if (mShowColliderDebug)
    {
        mColliderDebug.setPosition(transform.position + mCollider.getPosition());
        mColliderDebug.setSize(mCollider.getSize());
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

    if (mShowColliderDebug)
    {
        window->draw(mColliderDebug);
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

float GameEntity::getXVelocity()
{
    return mVelocity.x;
}

float GameEntity::getYVelocity()
{
    return mVelocity.y;
}

void GameEntity::setXVelocity(float x)
{
    mVelocity.x = x;
}

void GameEntity::setYVelocity(float y)
{
    mVelocity.y = y;
}

void GameEntity::setColliderSize(float width, float height)
{
    mCollider = sf::FloatRect(mCollider.left, mCollider.top, width, height);
}

void GameEntity::setColliderOffset(float x, float y)
{
    mCollider = sf::FloatRect(x, y, mCollider.width, mCollider.height);
}

void GameEntity::setColliderDebug(bool debug)
{
    mShowColliderDebug = debug;
}

bool GameEntity::getColliderDebug()
{
    return mShowColliderDebug;
}

sf::FloatRect GameEntity::getCollider()
{
    return sf::FloatRect(
        transform.position + mCollider.getPosition(),
        mCollider.getSize()
    );
}

void GameEntity::setSolid(bool solid)
{
    mSolid = solid;
}

bool GameEntity::getSolid()
{
    return mSolid;
}

bool GameEntity::checkCollision(std::shared_ptr<GameEntity> entity)
{
    for (auto& collider : mColliders)
    {
        if (collider->key == entity->key)
            return true;
    }

    return false;
}

void GameEntity::setZIndex(float zIndex)
{
    this->zIndex = zIndex;
}

float GameEntity::getZIndex()
{
    return zIndex;
}

GameEntityProperties& GameEntity::getDynamicProps()
{
    return mDynamicProps;
}