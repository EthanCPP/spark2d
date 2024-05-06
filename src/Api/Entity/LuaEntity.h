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

// TODO put these somewhere better
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

class LuaEntity
{
public:
	LuaEntity(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals);
	~LuaEntity();

	void setup(sol::state& lua);
	void setup(sol::state& lua, std::shared_ptr<GameEntity> entity);

	LuaApiMessage getMessage();
	std::vector<LuaApiPushState> getPushStates();

private:
	std::shared_ptr<SceneManager> mSceneManager;
	std::shared_ptr<sf::RenderWindow> mWindow;
	std::shared_ptr<SparkGlobals> mGlobals;
	std::shared_ptr<GameEntity> mEntity;

	LuaApiMessage mLuaApiMessage;
	std::vector<LuaApiPushState> mLuaApiPushStates;
};

