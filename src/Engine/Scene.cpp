#include "Scene.h"

Scene::Scene(std::string key)
{
	this->key = key;

	entityManager = std::make_shared<EntityManager>();

	mInitialised = false;
}

Scene::~Scene()
{

}

void Scene::init()
{
	mInitialised = true;
}

void Scene::start()
{
	if (!mInitialised) {
		init();
	}
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