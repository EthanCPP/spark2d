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

// TODO put somewhere better
struct LuaApiWaiter
{
	sol::function coroutine;
	float seconds;
	sf::Clock clock;
	bool complete = false;
};

class LuaTime
{
public:
	LuaTime(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals);
	~LuaTime();

	void setup(sol::state& lua);
	void update(float dt);

private:
	std::shared_ptr<SceneManager> mSceneManager;
	std::shared_ptr<sf::RenderWindow> mWindow;
	std::shared_ptr<SparkGlobals> mGlobals;

	// Coroutines
	std::shared_ptr<sol::state> mState;
	sol::function mCoroutineWrap;
	std::vector<LuaApiWaiter> mCoroutineWaiters;
};

