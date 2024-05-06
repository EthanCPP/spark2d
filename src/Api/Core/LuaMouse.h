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

class LuaMouse
{
public:
	LuaMouse(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals);
	~LuaMouse();

	void setup(sol::state& lua);
	void mouseDown(sol::state& lua, sf::Mouse::Button button);
	void mouseUp(sol::state& lua, sf::Mouse::Button button);
	void mouseWheelMoved(sol::state& lua, int direction);
	void mouseMoved(sol::state& lua, float x, float y);

private:
	std::shared_ptr<SceneManager> mSceneManager;
	std::shared_ptr<sf::RenderWindow> mWindow;
	std::shared_ptr<SparkGlobals> mGlobals;
};

