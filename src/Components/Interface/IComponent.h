#pragma once

#include "../Transform.h"

#include <SFML/Graphics.hpp>

#include <string>

class IComponent
{
public:
    virtual bool isDrawable() { return false; }
    virtual void update(Transform& globalTransform) {}
    virtual void render(std::shared_ptr<sf::RenderWindow> window) {}

protected:
    Transform mLocalTransform;
};