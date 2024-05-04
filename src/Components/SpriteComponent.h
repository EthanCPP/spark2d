#pragma once

#include "Interface/IComponent.h"
#include "Transform.h"
#include "../Engine/ResourceManager.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <memory>

enum SpriteAnimationFunction {
    Forward = 0,
    Boomerang = 1,
    Reverse = 2,
};

class SpriteComponent : public IComponent
{
public:
    SpriteComponent(std::string key);
    ~SpriteComponent();

    void init(ResourceManager* resourceManager);
    void loadTexture(std::string filepath);

    // animations (sprite sheets)
    void setFrameDimensions(unsigned int width, unsigned int height);
    void setRow(int row);
    void setFrame(int frame);
    void setMinFrame(int minFrame);
    void setMaxFrame(int maxFrame);
    void setFrameDuration(float duration);
    void playAnimation();
    void pauseAnimation();
    void setMaxAnimationIterations(int maxIterations);
    void setAnimationFunction(SpriteAnimationFunction animationFunction);


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

    // animations (sprite sheets)
    sf::Vector2u mFrameDimensions;
    int mRow, mMinFrame, mMaxFrame, mCurrentFrame;
    float mFrameDuration;
    sf::Clock mFrameTimer;
    bool mAnimationPlaying;
    int mCurrentIteration, mMaxIterations;
    SpriteAnimationFunction mAnimationFunction;
    bool mBoomerangReverse;

    void updateAnimation();
    void setSpriteRect();

};