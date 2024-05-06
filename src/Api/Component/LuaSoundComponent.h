#pragma once
#include "../../Engine/SceneManager.h"
#include "../../Engine/SparkGlobals.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>
#include <chrono>
#include <thread>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

class LuaSoundComponent
{
public:
	LuaSoundComponent(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals);
	~LuaSoundComponent();

	void setup(sol::state& lua);

private:
	std::shared_ptr<SceneManager> mSceneManager;
	std::shared_ptr<sf::RenderWindow> mWindow;
	std::shared_ptr<SparkGlobals> mGlobals;
};

