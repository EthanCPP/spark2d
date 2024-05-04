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

    virtual float getX() {
        return mLocalTransform.position.x;
    }

    virtual float getY() {
        return mLocalTransform.position.y;
    }

    virtual float getRotation() {
        return mLocalTransform.rotation;
    }

    virtual void setX(float x) {
        mLocalTransform.position.x = x;
    }

    virtual void setY(float y) {
        mLocalTransform.position.y = y;
    }

    virtual void setRotation(float angle) {
        mLocalTransform.rotation = angle;
    }

    virtual sf::Vector2i getSize() {
        return sf::Vector2i(0, 0);
    }

    virtual void setOrigin(sf::Vector2f origin) {}
    virtual sf::Vector2f getOrigin() { return sf::Vector2f(0.f, 0.f); }

protected:
    Transform mLocalTransform;
};