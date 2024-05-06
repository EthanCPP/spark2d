#pragma once
#include "../Engine/SceneManager.h"
#include "../Engine/SparkGlobals.h"

#include "Scene/LuaScene.h"
#include "Entity/LuaEntity.h"
#include "Component/LuaSpriteComponent.h"
#include "Component/LuaTextComponent.h"
#include "Component/LuaCircleComponent.h"
#include "Component/LuaRectangleComponent.h"
#include "Component/LuaSoundComponent.h"
#include "Core/LuaKeyboard.h"
#include "Core/LuaMouse.h"
#include "Core/LuaUtils.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>
#include <chrono>
#include <thread>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>


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
	LuaApi(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals);
	~LuaApi();

	void setup();
	void setup(std::shared_ptr<GameEntity> entity);
	bool init(std::string script);

	void update(std::vector<sf::Keyboard::Scancode> keysDown, float dt);
	void keyDown(sf::Keyboard::Scancode code);
	void keyUp(sf::Keyboard::Scancode code);
	void mouseDown(sf::Mouse::Button button);
	void mouseUp(sf::Mouse::Button button);
	void mouseWheelMoved(int direction);
	void mouseMoved(float x, float y);

	LuaApiMessage getMessage();
	std::vector<LuaApiPushState> getPushStates();

	std::vector<std::shared_ptr<GameEntity>> entitiesToDestroy;

	bool inactive;

private:
	sol::state lua;

	std::shared_ptr<SceneManager> mSceneManager;

	bool mEntityAttached;
	std::shared_ptr<GameEntity> mEntity;

	std::shared_ptr<SparkGlobals> mGlobals;

	float mMouseX, mMouseY;

	float mDt;
private:
	// Lua functions that call into C++
	void setupScene();
	void setupEntity();
	void setupSpriteComponent();
	void setupTextComponent();
	void setupCircleComponent();
	void setupRectangleComponent();
	void setupSoundComponent();
	void setupKeyboard();
	void setupMouse();
	void setupUtils();
	void setupTime();
	void setupGuiEntity();

	LuaScene* mLuaScene;
	LuaEntity* mLuaEntity;
	LuaSpriteComponent* mLuaSpriteComponent;
	LuaTextComponent* mLuaTextComponent;
	LuaCircleComponent* mLuaCircleComponent;
	LuaRectangleComponent* mLuaRectangleComponent;
	LuaSoundComponent* mLuaSoundComponent;
	LuaKeyboard* mLuaKeyboard;
	LuaMouse* mLuaMouse;
	LuaUtils* mLuaUtils;


	// Test functions
	void setupSandbox();
	std::shared_ptr<GameEntity> p;

	// Properties

	std::shared_ptr<sf::RenderWindow> mWindow;

	// Coroutines
	sol::function mCoroutineWrap;
	std::vector<LuaApiWaiter> mCoroutineWaiters;
};

