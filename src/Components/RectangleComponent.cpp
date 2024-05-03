#include "RectangleComponent.h"


RectangleComponent::RectangleComponent(std::string key)
{
    this->key = key;
    this->mHasTexture = false;
}

RectangleComponent::~RectangleComponent()
{

}

void RectangleComponent::init(ResourceManager* resourceManager)
{
    mResourceManager = resourceManager;
}


void RectangleComponent::setSize(float width, float height)
{
    mRectangleShape.setSize(sf::Vector2f(width, height));
}

void RectangleComponent::setFillColour(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
    mRectangleShape.setFillColor(sf::Color(r, g, b, a));
}

void RectangleComponent::setOutlineThickness(float thickness)
{
    mRectangleShape.setOutlineThickness(thickness);
}

void RectangleComponent::setOutlineColour(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
    mRectangleShape.setOutlineColor(sf::Color(r, g, b, a));
}

void RectangleComponent::setTexture(const std::string& filepath)
{
    mTexture = mResourceManager->getTexture(filepath);
    mRectangleShape.setTexture(mTexture.get());
    //mSprite.setOrigin(mSprite.getTextureRect().width / 2, mSprite.getTextureRect().height / 2);
}

void RectangleComponent::update(Transform& globalTransform)
{
    sf::Transform parentTransform;
    parentTransform.translate(globalTransform.position);
    parentTransform.rotate(globalTransform.rotation);

    sf::Transform childTransform;
    childTransform.translate(mLocalTransform.position);
    childTransform.rotate(mLocalTransform.rotation);

    mFinalTransform = parentTransform * childTransform;
}

void RectangleComponent::render(std::shared_ptr<sf::RenderWindow> window)
{
    window->draw(mRectangleShape, mFinalTransform);
}
