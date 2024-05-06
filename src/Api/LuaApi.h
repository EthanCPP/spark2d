#pragma once
#include "../Engine/SceneManager.h"
#include "../Engine/SparkGlobals.h"

#include "Scene/LuaScene.h"
#include "Entity/LuaEntity.h"
#include "Entity/LuaGuiEntity.h"
#include "Component/LuaSpriteComponent.h"
#include "Component/LuaTextComponent.h"
#include "Component/LuaCircleComponent.h"
#include "Component/LuaRectangleComponent.h"
#include "Component/LuaSoundComponent.h"
#include "Core/LuaKeyboard.h"
#include "Core/LuaMouse.h"
#include "Core/LuaUtils.h"
#include "Core/LuaTime.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>
#include <chrono>
#include <thread>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

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
	LuaScene* mLuaScene;
	LuaEntity* mLuaEntity;
	LuaGuiEntity* mLuaGuiEntity;
	LuaSpriteComponent* mLuaSpriteComponent;
	LuaTextComponent* mLuaTextComponent;
	LuaCircleComponent* mLuaCircleComponent;
	LuaRectangleComponent* mLuaRectangleComponent;
	LuaSoundComponent* mLuaSoundComponent;
	LuaKeyboard* mLuaKeyboard;
	LuaMouse* mLuaMouse;
	LuaUtils* mLuaUtils;
	LuaTime* mLuaTime;


	std::shared_ptr<sf::RenderWindow> mWindow;
};

