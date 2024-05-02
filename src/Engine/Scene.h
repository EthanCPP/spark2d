#pragma once
#include "ResourceManager.h"
#include "EntityManager.h"
#include "SparkGlobals.h"

#include <SFML/Graphics.hpp>

#include <memory>

class Scene
{
public:
	Scene(std::string key);
	~Scene();

	void init(ResourceManager* resourceManager, std::shared_ptr<SparkGlobals> sparkGlobals);
	void start();
	void stop();
	void update(float dt);
	void render(std::shared_ptr<sf::RenderWindow> window);

	std::shared_ptr<EntityManager> entityManager;

	std::string key;

private:
	bool mInitialised;

};

