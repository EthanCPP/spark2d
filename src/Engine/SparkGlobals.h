#pragma once
#include <string>
#include <SFML/Graphics.hpp>

struct SparkGlobals
{
    /*
    WINDOW
    */
    float windowWidth = 640;
    float windowHeight = 480;
    std::string windowTitle = "A cool spark game";
    float framerateLimit = 0;

    /*
    PHYSICS
    */
    sf::Vector2f gravity = sf::Vector2f(0.0f, 1.0f);
};
