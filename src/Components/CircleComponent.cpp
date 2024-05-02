#include "CircleComponent.h"


CircleComponent::CircleComponent(std::string key)
{
    this->key = key;
    this->mHasTexture = false;
}

CircleComponent::~CircleComponent()
{

}

void CircleComponent::init(ResourceManager* resourceManager)
{
    mResourceManager = resourceManager;
}


void CircleComponent::setRadius(float radius)
{
    mCircleShape.setRadius(radius);
}

void CircleComponent::setFillColour(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
    mCircleShape.setFillColor(sf::Color(r, g, b, a));
}

void CircleComponent::setOutlineThickness(float thickness)
{
    mCircleShape.setOutlineThickness(thickness);
}

void CircleComponent::setOutlineColour(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
    mCircleShape.setOutlineColor(sf::Color(r, g, b, a));
}

void CircleComponent::setTexture(const std::string& filepath)
{
    mTexture = mResourceManager->getTexture(filepath);
    mCircleShape.setTexture(mTexture.get());
    //mSprite.setOrigin(mSprite.getTextureRect().width / 2, mSprite.getTextureRect().height / 2);
}

void CircleComponent::update(Transform& globalTransform)
{
    sf::Transform parentTransform;
    parentTransform.translate(globalTransform.position.x, globalTransform.position.y);
    parentTransform.rotate(globalTransform.rotation.angle);

    sf::Transform childTransform;
    childTransform.translate(mLocalTransform.position.x, mLocalTransform.position.y);
    childTransform.rotate(mLocalTransform.rotation.angle);

    mFinalTransform = parentTransform * childTransform;
}

void CircleComponent::render(std::shared_ptr<sf::RenderWindow> window)
{
    window->draw(mCircleShape, mFinalTransform);
}
