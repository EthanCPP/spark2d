#include "EntityManager.h"

EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{

}

void EntityManager::addEntity(std::string key, std::shared_ptr<GameEntity> entity)
{
	mEntities.insert({ key, entity });
}

std::shared_ptr<GameEntity> EntityManager::getEntity(std::string key)
{
    return mEntities[key];
}

void EntityManager::update()
{
    for (auto const& entity : mEntities)
    {
        entity.second->update();
    }
}

void EntityManager::render(std::shared_ptr<sf::RenderWindow> window)
{
    for (auto const& entity : mEntities)
    {
        entity.second->render(window);
    }
}