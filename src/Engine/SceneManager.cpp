#include "SceneManager.h"

SceneManager::SceneManager(ResourceManager* resourceManager, std::shared_ptr<SparkGlobals> sparkGlobals)
{
	mResourceManager = resourceManager;
	mSparkGlobals = sparkGlobals;
}

SceneManager::~SceneManager()
{

}

void SceneManager::addScene(std::string key, std::shared_ptr<Scene> scene)
{
	scene->init(mResourceManager, mSparkGlobals);
	mScenes.insert({ key, scene });
}

void SceneManager::setScene(std::string key)
{
	if (! mCurrentScene.empty())
	{
		getCurrentScene()->stop();
	}

	mCurrentScene = key;

	getCurrentScene()->start();
}

std::shared_ptr<Scene> SceneManager::getScene(std::string key)
{
	return mScenes[key];
}

std::shared_ptr<Scene> SceneManager::getCurrentScene()
{
	return mScenes[mCurrentScene];
}

void SceneManager::update(float dt)
{
	if (!mCurrentScene.empty())
	{
		getCurrentScene()->update(dt);
	}
}

void SceneManager::render(std::shared_ptr<sf::RenderWindow> window)
{
	if (!mCurrentScene.empty())
	{
		getCurrentScene()->render(window);
	}
}