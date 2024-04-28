#pragma once
#include "../Engine/SceneManager.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>
#include <chrono>
#include <thread>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

enum LuaApiMessage
{
	NONE,
	PUSH_LUA_STATE,
};

struct LuaApiPushState
{
	std::shared_ptr<GameEntity> entity;
	std::string file;
};

struct LuaApiWaiter
{
	sol::function coroutine;
	float seconds;
	sf::Clock clock;
	bool complete = false;
};

class LuaApi
{
public:
	LuaApi(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window);
	~LuaApi();

	void setup();
	void setup(std::shared_ptr<GameEntity> entity);
	bool init(std::string script);

	void update(std::vector<sf::Keyboard::Scancode> keysDown);
	void keyDown(sf::Keyboard::Scancode code);
	void keyUp(sf::Keyboard::Scancode code);

	LuaApiMessage getMessage();
	std::vector<LuaApiPushState> getPushStates();

private:
	sol::state lua;

	std::shared_ptr<SceneManager> mSceneManager;
	std::shared_ptr<GameEntity> mEntity;

private:
	// Lua functions that call into C++
	void setupScene();
	void setupEntity();
	void setupSpriteComponent();
	void setupKeyboard();
	void setupUtils();
	void setupTime();


	// Test functions
	void setupSandbox();
	std::shared_ptr<GameEntity> p;

	// Properties
	std::vector<sf::Keyboard::Scancode> mKeysDown;

	LuaApiMessage mLuaApiMessage;
	std::vector<LuaApiPushState> mLuaApiPushStates;

	std::shared_ptr<sf::RenderWindow> mWindow;

	// Coroutines
	sol::function mCoroutineWrap;
	std::vector<LuaApiWaiter> mCoroutineWaiters;
};

