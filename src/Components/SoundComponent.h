#pragma once

#include "Interface/IComponent.h"
#include "Transform.h"
#include "../Engine/ResourceManager.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <string>
#include <memory>

class SoundComponent : public IComponent
{
public:
    SoundComponent(std::string key);
    ~SoundComponent();

    void init(ResourceManager* resourceManager);
    void loadSound(std::string filepath);

    bool isDrawable() { return false; }

    void update(Transform& globalTransform);

    void play();
    void setVolume(float volume);
    float getVolume();

    std::string key;
private:
    std::shared_ptr<sf::SoundBuffer> mSoundBuffer;
    sf::Sound mSound;

    sf::Transform mFinalTransform;

    ResourceManager* mResourceManager;
};