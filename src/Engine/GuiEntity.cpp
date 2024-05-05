#include "GuiEntity.h"

GuiEntity::GuiEntity()
{
}

GuiEntity::~GuiEntity()
{
}

void GuiEntity::init(ResourceManager* resourceManager, std::shared_ptr<SparkGlobals> sparkGlobals)
{
    mResourceManager = resourceManager;
    mSparkGlobals = sparkGlobals;

    zIndex = 0;
}

void GuiEntity::addSpriteComponent(std::shared_ptr<SpriteComponent> spriteComponent)
{
    // todo: safer way of keying
    spriteComponent->init(mResourceManager);
    mComponents.insert({ spriteComponent->key, spriteComponent });
}

void GuiEntity::addTextComponent(std::shared_ptr<TextComponent> textComponent)
{
    // todo: safer way of keying
    textComponent->init(mResourceManager);
    mComponents.insert({ textComponent->key, textComponent });
}

void GuiEntity::addCircleComponent(std::shared_ptr<CircleComponent> circleComponent)
{
    // todo: safer way of keying
    circleComponent->init(mResourceManager);
    mComponents.insert({ circleComponent->key, circleComponent });
}

void GuiEntity::addRectangleComponent(std::shared_ptr<RectangleComponent> rectangleComponent)
{
    // todo: safer way of keying
    rectangleComponent->init(mResourceManager);
    mComponents.insert({ rectangleComponent->key, rectangleComponent });
}

void GuiEntity::addSoundComponent(std::shared_ptr<SoundComponent> soundComponent)
{
    // todo: safer way of keying
    soundComponent->init(mResourceManager);
    mComponents.insert({ soundComponent->key, soundComponent });
}

std::shared_ptr<IComponent> GuiEntity::getComponent(std::string key)
{
    return mComponents[key];
}

void GuiEntity::update(float dt)
{
    for (auto const& component : mComponents)
    {
        component.second->update(transform);
    }
}

void GuiEntity::render(std::shared_ptr<sf::RenderWindow> window)
{
    for (auto const& component : mComponents)
    {
        if (component.second->isDrawable()) {
            component.second->render(window);
        }
    }
}

float GuiEntity::getX()
{
    return transform.position.x;
}

float GuiEntity::getY()
{
    return transform.position.y;
}

float GuiEntity::getRotation()
{
    return transform.rotation;
}

void GuiEntity::setX(float x)
{
    transform.position.x = x;
}

void GuiEntity::setY(float y)
{
    transform.position.y = y;
}

void GuiEntity::setRotation(float angle)
{
    transform.rotation = angle;
}

void GuiEntity::setZIndex(float zIndex)
{
    this->zIndex = zIndex;
}

float GuiEntity::getZIndex()
{
    return zIndex;
}
