#include "LuaManager.h"

LuaManager::LuaManager(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals)
{
	mSceneManager = sceneManager;
	mWindow = window;
	mGlobals = globals;
}

LuaManager::~LuaManager()
{
}

bool LuaManager::addLuaState(std::string file)
{
	std::shared_ptr<LuaApi> luaState = std::make_shared<LuaApi>(mSceneManager, mWindow, mGlobals);
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
	std::shared_ptr<LuaApi> luaState = std::make_shared<LuaApi>(mSceneManager, mWindow, mGlobals);
	luaState->setup(entity);

	if (!luaState->init(file))
	{
		std::cout << "Error loading file " << file << " on entity" << std::endl;
		return false;
	}

	mLuaStates.push_back(luaState);
	entity->addLuaState(luaState);

	return true;
}

void LuaManager::update(float dt)
{
	mDt = dt;

	// the size of the vector can change through the iteration
	for (std::size_t i = 0; i != mLuaStates.size(); ++i)
	{
		if (mLuaStates[i]->inactive)
			continue;

		mLuaStates[i]->update(mKeysDown, mDt);

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

		if (!mLuaStates[i]->entitiesToDestroy.empty())
		{
			for (auto entityToDestroy : mLuaStates[i]->entitiesToDestroy)
			{
				// Deactivate all Lua states associated with this entity. These objects will get destroyed when the scene ends.
				for (auto luaState : entityToDestroy->mLuaStates)
				{
					luaState->inactive = true;
				}

				mSceneManager->getScene(entityToDestroy->sceneKey)->entityManager->destroyEntity(entityToDestroy->key);
			}

			mLuaStates[i]->entitiesToDestroy.clear();
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

void LuaManager::mouseDown(sf::Mouse::Button button)
{
	for (auto luaState : mLuaStates)
	{
		luaState->mouseDown(button);
	}
}

void LuaManager::mouseUp(sf::Mouse::Button button)
{
	for (auto luaState : mLuaStates)
	{
		luaState->mouseUp(button);
	}
}

void LuaManager::mouseWheelMoved(int direction)
{
	for (auto luaState : mLuaStates)
	{
		luaState->mouseWheelMoved(direction);
	}
}

void LuaManager::mouseMoved(float x, float y)
{
	for (auto luaState : mLuaStates)
	{
		luaState->mouseMoved(x, y);
	}
}