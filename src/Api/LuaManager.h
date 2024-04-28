#pragma once
#include "LuaApi.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

class LuaManager
{
public:
	LuaManager(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window);
	~LuaManager();

	bool addLuaState(std::string file);
	bool addLuaState(std::string file, std::shared_ptr<GameEntity> entity);

	void update();
	void keyDown(sf::Keyboard::Scancode code);
	void keyUp(sf::Keyboard::Scancode code);

private:
	std::shared_ptr<SceneManager> mSceneManager;
	std::vector<std::shared_ptr<LuaApi>> mLuaStates;

	std::shared_ptr<sf::RenderWindow> mWindow;

	std::vector<sf::Keyboard::Scancode> mKeysDown;
};

