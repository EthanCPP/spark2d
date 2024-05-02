#include "Scene.h"

Scene::Scene(std::string key)
{
	this->key = key;
	mInitialised = false;
}

Scene::~Scene()
{

}

void Scene::init(ResourceManager* resourceManager, std::shared_ptr<SparkGlobals> sparkGlobals)
{
	entityManager = std::make_shared<EntityManager>(resourceManager, sparkGlobals);

	mInitialised = true;
}

void Scene::start()
{

}

void Scene::stop()
{

}

void Scene::update(float dt)
{
	entityManager->update(dt);
}

void Scene::render(std::shared_ptr<sf::RenderWindow> window)
{
	entityManager->render(window);
}