#pragma once
#include "../Components/Interface/IComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TextComponent.h"
#include "../Components/CircleComponent.h"
#include "../Components/RectangleComponent.h"
#include "../Components/SoundComponent.h"
#include "../Components/Transform.h"
#include "ResourceManager.h"
#include "SparkGlobals.h"

#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>
#include <map>
#include <string>
#include <unordered_map>

class GuiEntity
{
public:
    GuiEntity();
    ~GuiEntity();

    void init(ResourceManager* resourceManager, std::shared_ptr<SparkGlobals> sparkGlobals);

    void addSpriteComponent(std::shared_ptr<SpriteComponent> spriteComponent);
    void addTextComponent(std::shared_ptr<TextComponent> textComponent);
    void addCircleComponent(std::shared_ptr<CircleComponent> circleComponent);
    void addRectangleComponent(std::shared_ptr<RectangleComponent> rectangleComponent);
    void addSoundComponent(std::shared_ptr<SoundComponent> soundComponent);

    std::shared_ptr<IComponent> getComponent(std::string key);

    void update(float dt);
    void render(std::shared_ptr<sf::RenderWindow> window);

    float getX();
    float getY();
    float getRotation();

    void setX(float x);
    void setY(float y);
    void setRotation(float angle);

    float zIndex;
    void setZIndex(float zIndex);
    float getZIndex();

    std::string key;
    std::string sceneKey;

private:
    std::map<std::string, std::shared_ptr<IComponent>> mComponents;

    Transform transform;
    ResourceManager* mResourceManager;
    std::shared_ptr<SparkGlobals> mSparkGlobals;
};

