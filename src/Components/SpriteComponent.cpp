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

    mSprite.setOrigin(mSprite.getTextureRect().width / 2, mSprite.getTextureRect().height / 2);
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::update(Transform& globalTransform)
{
    sf::Transform parentTransform;
    parentTransform.translate(globalTransform.position.x, globalTransform.position.y);
    parentTransform.rotate(globalTransform.rotation.angle);

    sf::Transform childTransform;
    childTransform.translate(mLocalTransform.position.x, mLocalTransform.position.y);
    childTransform.rotate(mLocalTransform.rotation.angle);

    mFinalTransform = parentTransform * childTransform;
}

void SpriteComponent::render(std::shared_ptr<sf::RenderWindow> window)
{
    window->draw(mSprite, mFinalTransform);

}
