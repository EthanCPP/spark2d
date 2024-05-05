#pragma once
#include "ResourceManager.h"
#include "SparkGlobals.h"
#include "GuiEntity.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <string>
#include <map>

class GuiManager
{
public:
	GuiManager(ResourceManager* resourceManager, std::shared_ptr<SparkGlobals> sparkGlobals);
	~GuiManager();

	void addEntity(std::string key, std::shared_ptr<GuiEntity> entity);
	std::shared_ptr<GuiEntity> getEntity(std::string key);
	void destroyEntity(std::string key);

	void update(float dt);
	void render(std::shared_ptr<sf::RenderWindow> window);

	void sortZIndexes();

private:
	std::map<std::string, std::shared_ptr<GuiEntity>> mEntities;
	ResourceManager* mResourceManager;
	std::shared_ptr<SparkGlobals> mSparkGlobals;

	std::vector<std::shared_ptr<GuiEntity>> mEntitiesSorted; // sorted by zIndex ready for rendering

	static bool zIndexSortFunc(std::shared_ptr<GuiEntity> i, std::shared_ptr<GuiEntity> j);

	sf::Clock mSortClock;
};

