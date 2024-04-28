#pragma once

#include "Interface/IComponent.h"
#include "Transform.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

class SpriteComponent : public IComponent
{
public:
    SpriteComponent(std::string key, std::string filepath);
    ~SpriteComponent();

    virtual bool isDrawable() { return true; }

    void update(Transform& globalTransform);
    void render(std::shared_ptr<sf::RenderWindow> window);

    std::string key;
private:
    sf::Texture mTexture;
    sf::Sprite mSprite;

    sf::CircleShape debugPoint;

};