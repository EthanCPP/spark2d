#include "TextComponent.h"


TextComponent::TextComponent(std::string key)
{
    this->key = key;
}

TextComponent::~TextComponent()
{

}

void TextComponent::init(ResourceManager* resourceManager)
{
    mResourceManager = resourceManager;
}

void TextComponent::loadFont(std::string filepath)
{
    mFont = mResourceManager->getFont(filepath);
    mText.setFont(*mFont);
    //mSprite.setOrigin(mSprite.getTextureRect().width / 2, mSprite.getTextureRect().height / 2);
}

void TextComponent::setText(const std::string& text)
{
    mText.setString(text);
}

void TextComponent::setCharacterSize(unsigned int size)
{
    mText.setCharacterSize(size);
}

void TextComponent::setFillColour(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
    mText.setFillColor(sf::Color(r, g, b, a));
}

void TextComponent::setBold(bool bold)
{
    mStyleBold = bold;
    this->applyStyles();
}

void TextComponent::setUnderlined(bool underlined)
{
    mStyleUnderlined = underlined;
    this->applyStyles();
}

void TextComponent::setItalic(bool italic)
{
    mStyleItalic = italic;
    this->applyStyles();
}

void TextComponent::applyStyles()
{
    sf::Uint32 style = 0;

    if (mStyleBold)
        style += sf::Text::Bold;

    if (mStyleUnderlined)
        style += sf::Text::Underlined;

    if (mStyleItalic)
        style += sf::Text::Italic;

    mText.setStyle(style);
}

void TextComponent::update(Transform& globalTransform)
{
    sf::Transform parentTransform;
    parentTransform.translate(globalTransform.position.x, globalTransform.position.y);
    parentTransform.rotate(globalTransform.rotation.angle);

    sf::Transform childTransform;
    childTransform.translate(mLocalTransform.position.x, mLocalTransform.position.y);
    childTransform.rotate(mLocalTransform.rotation.angle);

    mFinalTransform = parentTransform * childTransform;
}

void TextComponent::render(std::shared_ptr<sf::RenderWindow> window)
{
    window->draw(mText, mFinalTransform);
}
