#include "GuiManager.h"


GuiManager::GuiManager(ResourceManager* resourceManager, std::shared_ptr<SparkGlobals> sparkGlobals)
{
    mResourceManager = resourceManager;
    mSparkGlobals = sparkGlobals;
}

GuiManager::~GuiManager()
{

}

void GuiManager::addEntity(std::string key, std::shared_ptr<GuiEntity> entity)
{
    entity->init(mResourceManager, mSparkGlobals);
    mEntities.insert({ key, entity });

    sortZIndexes();
}

std::shared_ptr<GuiEntity> GuiManager::getEntity(std::string key)
{
    return mEntities[key];
}

void GuiManager::destroyEntity(std::string key)
{
    mEntities.erase(key);
}

void GuiManager::update(float dt)
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
        entity.second->update(dt);
    }
}

void GuiManager::render(std::shared_ptr<sf::RenderWindow> window)
{
    for (auto const& entity : mEntitiesSorted)
    {
        entity->render(window);
    }
}

void GuiManager::sortZIndexes()
{
    mEntitiesSorted.clear();

    for (auto const& entity : mEntities)
        mEntitiesSorted.emplace_back(entity.second);

    std::sort(mEntitiesSorted.begin(), mEntitiesSorted.end(), &zIndexSortFunc);
}

bool GuiManager::zIndexSortFunc(std::shared_ptr<GuiEntity> i, std::shared_ptr<GuiEntity> j)
{
    return i->zIndex < j->zIndex;
}