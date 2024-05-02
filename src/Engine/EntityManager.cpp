#include "EntityManager.h"

EntityManager::EntityManager(ResourceManager* resourceManager, std::shared_ptr<SparkGlobals> sparkGlobals)
{
    mResourceManager = resourceManager;
    mSparkGlobals = sparkGlobals;
}

EntityManager::~EntityManager()
{

}

void EntityManager::addEntity(std::string key, std::shared_ptr<GameEntity> entity)
{
    entity->init(mResourceManager, mSparkGlobals);
	mEntities.insert({ key, entity });
}

std::shared_ptr<GameEntity> EntityManager::getEntity(std::string key)
{
    return mEntities[key];
}

void EntityManager::destroyEntity(std::string key)
{
    mEntities.erase(key);
}

void EntityManager::update(float dt)
{
    for (auto const& entity : mEntities)
    {
        entity.second->update(dt);
    }
}

void EntityManager::render(std::shared_ptr<sf::RenderWindow> window)
{
    for (auto const& entity : mEntities)
    {
        entity.second->render(window);
    }
}