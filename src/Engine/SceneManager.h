#pragma once
#include "Scene.h"
#include "ResourceManager.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <map>
#include <string>

class SceneManager
{
public:
	SceneManager(ResourceManager* resourceManager);
	~SceneManager();

	void addScene(std::string key, std::shared_ptr<Scene> scene);
	void setScene(std::string key);

	std::shared_ptr<Scene> getScene(std::string key);
	std::shared_ptr<Scene> getCurrentScene();

	void update();
	void render(std::shared_ptr<sf::RenderWindow> window);
private:
	std::string mCurrentScene;
	std::map<std::string, std::shared_ptr<Scene>> mScenes;

	ResourceManager* mResourceManager;
};

