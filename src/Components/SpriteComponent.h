#pragma once

#include "Interface/IComponent.h"
#include "Transform.h"
#include "../Engine/ResourceManager.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <memory>

class SpriteComponent : public IComponent
{
public:
    SpriteComponent(std::string key);
    ~SpriteComponent();

    void init(ResourceManager* resourceManager);
    void loadTexture(std::string filepath);

    sf::Vector2i getSize();
    void setOrigin(sf::Vector2f origin);
    sf::Vector2f getOrigin();

    virtual bool isDrawable() { return true; }

    void update(Transform& globalTransform);
    void render(std::shared_ptr<sf::RenderWindow> window);

    std::string key;
private:
    std::shared_ptr<sf::Texture> mTexture;
    sf::Sprite mSprite;

    sf::Transform mFinalTransform;

    ResourceManager* mResourceManager;
};