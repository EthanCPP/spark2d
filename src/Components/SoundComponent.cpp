#include "SoundComponent.h"


SoundComponent::SoundComponent(std::string key)
{
    this->key = key;
}

SoundComponent::~SoundComponent()
{
}

void SoundComponent::init(ResourceManager* resourceManager)
{
    mResourceManager = resourceManager;
}

void SoundComponent::loadSound(std::string filepath)
{
    mSoundBuffer = mResourceManager->getSoundBuffer(filepath);
    mSound.setBuffer(*mSoundBuffer);
}

void SoundComponent::update(Transform& globalTransform)
{
}

void SoundComponent::play()
{
    mSound.play();
}

void SoundComponent::setVolume(float volume)
{
    mSound.setVolume(volume);
}

float SoundComponent::getVolume()
{
    return mSound.getVolume();
}