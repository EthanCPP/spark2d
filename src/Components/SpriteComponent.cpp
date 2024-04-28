#include "SpriteComponent.h"


SpriteComponent::SpriteComponent(std::string key, std::string filepath)
{
    this->key = key;

    if (!mTexture.loadFromFile("data/" + filepath))
    {
        std::cout << "SpriteComponent: Unable to load texture from file: " << filepath << std::endl;
    }
    else
    {
        mSprite.setTexture(mTexture);
    }
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::update(Transform& globalTransform)
{
    mSprite.setPosition(sf::Vector2f(
        globalTransform.position.x + mLocalTransform.position.x,
        globalTransform.position.y + mLocalTransform.position.y
    ));
}

void SpriteComponent::render(std::shared_ptr<sf::RenderWindow> window)
{
    window->draw(mSprite);
}
