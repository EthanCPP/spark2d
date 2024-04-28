#include "LuaManager.h"

LuaManager::LuaManager(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window)
{
	mSceneManager = sceneManager;
	mWindow = window;
}

LuaManager::~LuaManager()
{
}

bool LuaManager::addLuaState(std::string file)
{
	std::shared_ptr<LuaApi> luaState = std::make_shared<LuaApi>(mSceneManager, mWindow);
	luaState->setup();

	if (!luaState->init(file))
	{
		std::cout << "Error loading file " << file << std::endl;
		return false;
	}

	mLuaStates.push_back(luaState);

	return true;
}

bool LuaManager::addLuaState(std::string file, std::shared_ptr<GameEntity> entity)
{
	std::shared_ptr<LuaApi> luaState = std::make_shared<LuaApi>(mSceneManager, mWindow);
	luaState->setup(entity);

	if (!luaState->init(file))
	{
		std::cout << "Error loading file " << file << " on entity" << std::endl;
		return false;
	}

	mLuaStates.push_back(luaState);

	return true;
}

void LuaManager::update()
{
	// the size of the vector can change through the iteration
	for (std::size_t i = 0; i != mLuaStates.size(); ++i)
	{
		mLuaStates[i]->update(mKeysDown);

		LuaApiMessage message = mLuaStates[i]->getMessage();

		if (message == LuaApiMessage::PUSH_LUA_STATE)
		{
			// The Lua state wants to spawn another one
			std::vector<LuaApiPushState> pushStates = mLuaStates[i]->getPushStates();

			for (auto pushState : pushStates)
			{
				// todo what if entity is null?
				if (!this->addLuaState("data/scripts/" + pushState.file, pushState.entity))
				{
					std::cout << "Unable to parse file " << pushState.file << std::endl;
				}
			}
		}
	}
}

void LuaManager::keyDown(sf::Keyboard::Scancode code)
{
	if (std::find(mKeysDown.begin(), mKeysDown.end(), code) == mKeysDown.end())
	{
		// Add to the keys down list
		mKeysDown.push_back(code);

		for (auto luaState : mLuaStates)
		{
			luaState->keyDown(code);
		}
	}
}

void LuaManager::keyUp(sf::Keyboard::Scancode code)
{
	auto i = std::find(mKeysDown.begin(), mKeysDown.end(), code);

	if (i != mKeysDown.end())
	{
		// Remove from the keys down list
		mKeysDown.erase(i);

		for (auto luaState : mLuaStates)
		{
			luaState->keyUp(code);
		}
	}
}