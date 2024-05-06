#include "LuaTime.h"


LuaTime::LuaTime(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals)
{
    mSceneManager = sceneManager;
    mWindow = window;
    mGlobals = globals;
}

LuaTime::~LuaTime()
{

}

void LuaTime::setup(sol::state& lua)
{
        lua["spark"]["time"] = lua.create_table_with(
        "delta", 0.01f
    );

    lua["spark"]["time"]["wait"] = sol::yielding([this](sol::table time, sol::function f, float seconds) {
        LuaApiWaiter waiter;
        waiter.coroutine = f;
        waiter.seconds = seconds;
        waiter.clock.restart();

        mCoroutineWaiters.push_back(waiter);
        });

    mCoroutineWrap = lua["coroutine"]["wrap"];
    lua["coroutine"]["wrap"] = [this](sol::function f, sol::this_state ts) {
        sol::function co = mCoroutineWrap(f);
        return co;
    };
}

void LuaTime::update(float dt)
{
    /*
    Go through all coroutines that are currently waiting and determine if they have expired
    TODO: find an efficient way of removing these from the vector that doesn't cause memory issues
    */
    if (!mCoroutineWaiters.empty())
    {
        for (auto& waiter : mCoroutineWaiters)
        {
            if (waiter.complete)
                continue;

            if (waiter.clock.getElapsedTime().asSeconds() >= waiter.seconds)
            {
                waiter.complete = true;

                sol::function co = mCoroutineWrap(waiter.coroutine);
                co();
            }
        }
    }
}