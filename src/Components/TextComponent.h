#pragma once
#include "Interface/IComponent.h"
#include "Transform.h"
#include "../Engine/ResourceManager.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <memory>

class TextComponent : public IComponent
{
public:
    TextComponent(std::string key);
    ~TextComponent();

    void init(ResourceManager* resourceManager);
    void loadFont(std::string filepath);
    void setText(const std::string& text);
    void setCharacterSize(unsigned int size);
    void setFillColour(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
    void setBold(bool bold);
    void setUnderlined(bool underlined);
    void setItalic(bool italic);

    virtual bool isDrawable() { return true; }

    void update(Transform& globalTransform);
    void render(std::shared_ptr<sf::RenderWindow> window);

    std::string key;
private:
    std::shared_ptr<sf::Font> mFont;
    sf::Text mText;

    sf::Transform mFinalTransform;

    ResourceManager* mResourceManager;

    void applyStyles();
    bool mStyleBold, mStyleUnderlined, mStyleItalic;
};