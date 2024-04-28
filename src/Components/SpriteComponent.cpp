#include "SpriteComponent.h"


SpriteComponent::SpriteComponent(std::string key, std::string filepath)
    : debugPoint(2)
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

    mSprite.setOrigin(mSprite.getTextureRect().width / 2, mSprite.getTextureRect().height / 2);
    debugPoint.setFillColor(sf::Color(255, 0, 0));
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

    mSprite.setRotation(globalTransform.rotation.angle + mLocalTransform.rotation.angle);


    debugPoint.setPosition(globalTransform.position.x, globalTransform.position.y);
}

void SpriteComponent::render(std::shared_ptr<sf::RenderWindow> window)
{
    window->draw(mSprite);

    window->draw(debugPoint);

}
