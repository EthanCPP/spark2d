#pragma once
#include "ResourceManager.h"
#include "GameEntity.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <string>
#include <map>

class EntityManager
{
public:
	EntityManager(ResourceManager* resourceManager);
	~EntityManager();

	void addEntity(std::string key, std::shared_ptr<GameEntity> entity);
	std::shared_ptr<GameEntity> getEntity(std::string key);

	void update();
	void render(std::shared_ptr<sf::RenderWindow> window);

private:
	std::map<std::string, std::shared_ptr<GameEntity>> mEntities;
	ResourceManager* mResourceManager;
};

