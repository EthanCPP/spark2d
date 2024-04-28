#pragma once
#include "EntityManager.h"

#include <SFML/Graphics.hpp>

#include <memory>

class Scene
{
public:
	Scene(std::string key);
	~Scene();

	void start();
	void stop();
	void update();
	void render(std::shared_ptr<sf::RenderWindow> window);

	std::shared_ptr<EntityManager> entityManager;

	std::string key;
protected:
	void init();

private:
	bool mInitialised;

};

