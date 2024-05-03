#pragma once
#include <SFML/Graphics.hpp>

struct Transform
{
	float rotation = 0;
	sf::Vector2f position = sf::Vector2f(0.f, 0.f);
};