#pragma once
#include "ResourceManager.h"
#include "GameEntity.h"
#include "SparkGlobals.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <string>
#include <map>

class EntityManager
{
public:
	EntityManager(ResourceManager* resourceManager, std::shared_ptr<SparkGlobals> sparkGlobals);
	~EntityManager();

	void addEntity(std::string key, std::shared_ptr<GameEntity> entity);
	std::shared_ptr<GameEntity> getEntity(std::string key);
	void destroyEntity(std::string key);

	void update(float dt);
	void render(std::shared_ptr<sf::RenderWindow> window);

private:
	std::map<std::string, std::shared_ptr<GameEntity>> mEntities;
	ResourceManager* mResourceManager;
	std::shared_ptr<SparkGlobals> mSparkGlobals;
};

