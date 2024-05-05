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
    mBoundary = sf::FloatRect(0.f, 0.f, 0.f, 0.f);
    mShowBoundaryDebug = false;
    mBoundaryDebug.setOutlineColor(sf::Color(255, 0, 0));
    mBoundaryDebug.setFillColor(sf::Color(0, 0, 0, 0));
    mBoundaryDebug.setOutlineThickness(2);
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

void GuiEntity::update(float dt, float mouseX, float mouseY)
{
    for (auto const& component : mComponents)
    {
        component.second->update(transform);
    }

    if (mShowBoundaryDebug)
    {
        mBoundaryDebug.setPosition(transform.position + mBoundary.getPosition());
        mBoundaryDebug.setSize(mBoundary.getSize());
    }

    sf::FloatRect boundary = getBoundary();
    mMouseOver = mouseX >= boundary.left
        && mouseX <= boundary.left + boundary.width
        && mouseY >= boundary.top
        && mouseY <= boundary.top + boundary.height;
}

void GuiEntity::render(std::shared_ptr<sf::RenderWindow> window)
{
    for (auto const& component : mComponents)
    {
        if (component.second->isDrawable()) {
            component.second->render(window);
        }
    }

    if (mShowBoundaryDebug)
    {
        window->draw(mBoundaryDebug);
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

bool GuiEntity::getMouseOver()
{
    return mMouseOver;
}

void GuiEntity::setBoundarySize(float width, float height)
{
    mBoundary = sf::FloatRect(mBoundary.left, mBoundary.top, width, height);
}

void GuiEntity::setBoundaryOffset(float x, float y)
{
    mBoundary = sf::FloatRect(x, y, mBoundary.width, mBoundary.height);
}

void GuiEntity::setBoundaryDebug(bool debug)
{
    mShowBoundaryDebug = debug;
}

bool GuiEntity::getBoundaryDebug()
{
    return mShowBoundaryDebug;
}

sf::FloatRect GuiEntity::getBoundary()
{
    return sf::FloatRect(
        transform.position + mBoundary.getPosition(),
        mBoundary.getSize()
    );
}