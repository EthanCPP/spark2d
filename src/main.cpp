#include "Engine/SceneManager.h"
#include "Api/LuaApi.h"
#include "Api/LuaManager.h"

#include <SFML/Graphics.hpp>

#include <memory>


int main()
{
    std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(sf::VideoMode(640, 480), "A cool spark game");

    std::shared_ptr<SceneManager> sceneManager = std::make_shared<SceneManager>();
    std::shared_ptr<LuaManager> luaManager = std::make_shared<LuaManager>(sceneManager, window);

    if (!luaManager->addLuaState("data/scripts/spark.lua"))
    {
        // Failed loading script
        return 0;
    }

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();

            if (event.type == sf::Event::KeyPressed)
                luaManager->keyDown(event.key.scancode);

            if (event.type == sf::Event::KeyReleased)
                luaManager->keyUp(event.key.scancode);
        }

        luaManager->update();
        sceneManager->update();

        window->clear();

        sceneManager->render(window);

        window->display();
    }


    return 0;
}