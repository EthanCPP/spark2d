#include "LuaScene.h"


LuaScene::LuaScene(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals)
{
	mSceneManager = sceneManager;
	mWindow = window;
	mGlobals = globals;
}

LuaScene::~LuaScene()
{

}

void LuaScene::setup(sol::state& lua)
{
    lua.new_usertype<SceneProperties>("SceneProperties",
        sol::meta_function::new_index, &SceneProperties::setPropertyLua,
        sol::meta_function::index, &SceneProperties::getPropertyLua
    );

    /*
    * =========================================
    * Create a new scene object
    * =========================================
    */

    // -- local scene = Scene.new("key")
    lua.new_usertype<Scene>("Scene",
        sol::meta_function::construct,
        sol::factories(
            [this](std::string key) {
                std::shared_ptr<Scene> scene = std::make_shared<Scene>(key);
                mSceneManager->addScene(key, scene);

                return scene;
            }
        ),
        "props", sol::property(&Scene::getDynamicProps)
    );

    /*
    * =========================================
    * Start the scene
    * =========================================
    */

    // -- scene:start()
    lua["Scene"]["start"] = [this](Scene& scene)
    {
        mSceneManager->setScene(scene.key);
    };

    /*
    * =========================================
    * Create a new game entity object directly on the scene
    * =========================================
    */

    // -- scene:createEntity("key")
    lua["Scene"]["createEntity"] = [this](Scene& scene, std::string key) {
        std::shared_ptr<GameEntity> entity = std::make_shared<GameEntity>();
        entity->sceneKey = scene.key;
        entity->key = key;

        scene.entityManager->addEntity(key, entity);

        return entity;
    };

    lua["Scene"]["getEntity"] = [this](Scene& scene, std::string key) {
        return scene.entityManager->getEntity(key);
    };

    /*
    * =========================================
    * Create a new gui entity object directly on the scene
    * =========================================
    */

    // -- scene:createGuiEntity("key")
    lua["Scene"]["createGuiEntity"] = [this](Scene& scene, std::string key) {
        std::shared_ptr<GuiEntity> entity = std::make_shared<GuiEntity>();
        entity->sceneKey = scene.key;
        entity->key = key;

        scene.guiManager->addEntity(key, entity);

        return entity;
    };

    lua["Scene"]["getGuiEntity"] = [this](Scene& scene, std::string key) {
        return scene.guiManager->getEntity(key);
    };

    lua["spark"]["getScene"] = [this](sol::table spark, std::string key) {
        return mSceneManager->getScene(key);
    };
}