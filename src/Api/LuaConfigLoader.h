#pragma once
#include "../Engine/SparkGlobals.h"
#include "../Engine/ResourceManager.h"

#include <memory>
#include <vector>
#include <chrono>
#include <thread>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

class LuaConfigLoader
{
public:
	LuaConfigLoader(std::shared_ptr<SparkGlobals> globals, ResourceManager* resourceManager);
	~LuaConfigLoader();

	void setup();
	bool init(std::string script);

private:
	void setupWindowSettings();
	void setupFunctions();

private:
	sol::state lua;

	std::shared_ptr<SparkGlobals> mGlobals;

	ResourceManager* mResourceManager;
};

