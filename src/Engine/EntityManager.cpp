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

    sortZIndexes();
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
    // Re-sorting zindexes every second.
    // todo: can we do this better
    if (mSortClock.getElapsedTime().asSeconds() >= 1)
    {
        mSortClock.restart();
        sortZIndexes();
    }

    for (auto const& entity : mEntities)
    {
        entity.second->mColliders.clear();

        for (auto const& otherEntity : mEntities)
        {
            if (entity.first == otherEntity.first)
                continue;

            const sf::FloatRect entityCollider = entity.second->getCollider();
            const sf::FloatRect otherEntityCollider = otherEntity.second->getCollider();

            if (entityCollider.left + entityCollider.width >= otherEntityCollider.left &&
                entityCollider.top + entityCollider.height >= otherEntityCollider.top &&
                entityCollider.left <= otherEntityCollider.left + otherEntityCollider.width &&
                entityCollider.top <= otherEntityCollider.top + otherEntityCollider.height)
            {
                entity.second->mColliders.push_back(otherEntity.second);
            }
        }

        entity.second->update(dt);
    }
}

void EntityManager::sortZIndexes()
{
    mEntitiesSorted.clear();

    for (auto const& entity : mEntities)
        mEntitiesSorted.emplace_back(entity.second);

    std::sort(mEntitiesSorted.begin(), mEntitiesSorted.end(), &zIndexSortFunc);
}

void EntityManager::render(std::shared_ptr<sf::RenderWindow> window)
{
    for (auto const& entity : mEntitiesSorted)
    {
        entity->render(window);
    }
}

bool EntityManager::zIndexSortFunc(std::shared_ptr<GameEntity> i, std::shared_ptr<GameEntity> j)
{
    return i->zIndex < j->zIndex;
}