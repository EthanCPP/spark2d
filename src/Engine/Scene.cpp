#include "Scene.h"

Scene::Scene(std::string key)
{
	this->key = key;
	mInitialised = false;
}

Scene::~Scene()
{

}

void Scene::init(ResourceManager* resourceManager)
{
	entityManager = std::make_shared<EntityManager>(resourceManager);

	mInitialised = true;
}

void Scene::start()
{

}

void Scene::stop()
{

}

void Scene::update()
{
	entityManager->update();
}

void Scene::render(std::shared_ptr<sf::RenderWindow> window)
{
	entityManager->render(window);
}