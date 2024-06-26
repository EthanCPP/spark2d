#include "ResourceManager.h"

ResourceManager::ResourceManager(std::shared_ptr<SparkGlobals> globals)
{
    mGlobals = globals;
}

ResourceManager::~ResourceManager()
{

}

std::shared_ptr<sf::Texture> ResourceManager::getTexture(std::string path)
{
    if (mGlobals->isDebug)
        path = "x64/Debug/data/" + path;
    else
        path = "data/" + path;

    if (mTextures.find(path) != mTextures.end())
    {
        // Already exists - just return the item
        return mTextures[path];
    }

    std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();

    if (!texture->loadFromFile(path))
    {
        std::cout << "Unable to load texture from file: " << path << std::endl;
    }

    mTextures.insert({ path, texture });

    return texture;
}

void ResourceManager::unloadTexture(std::string path)
{
    if (mGlobals->isDebug)
        path = "x64/Debug/data/" + path;
    else
        path = "data/" + path;

    if (mTextures.find(path) != mTextures.end())
    {
        mTextures.erase(path);
    }
}

std::shared_ptr<sf::Font> ResourceManager::getFont(std::string path)
{
    if (mGlobals->isDebug)
        path = "x64/Debug/data/" + path;
    else
        path = "data/" + path;

    if (mFonts.find(path) != mFonts.end())
    {
        // Already exists - just return the item
        return mFonts[path];
    }

    std::shared_ptr<sf::Font> font = std::make_shared<sf::Font>();

    if (!font->loadFromFile(path))
    {
        std::cout << "Unable to load font from file: " << path << std::endl;
    }

    mFonts.insert({ path, font });

    return font;
}

void ResourceManager::unloadFont(std::string path)
{
    if (mGlobals->isDebug)
        path = "x64/Debug/data/" + path;
    else
        path = "data/" + path;

    if (mFonts.find(path) != mFonts.end())
    {
        mFonts.erase(path);
    }
}

std::shared_ptr<sf::SoundBuffer> ResourceManager::getSoundBuffer(std::string path)
{
    if (mGlobals->isDebug)
        path = "x64/Debug/data/" + path;
    else
        path = "data/" + path;

    if (mSoundBuffers.find(path) != mSoundBuffers.end())
    {
        // Already exists - just return the item
        return mSoundBuffers[path];
    }

    std::shared_ptr<sf::SoundBuffer> soundBuffer = std::make_shared<sf::SoundBuffer>();

    if (!soundBuffer->loadFromFile(path))
    {
        std::cout << "Unable to load sound from file: " << path << std::endl;
    }

    mSoundBuffers.insert({ path, soundBuffer });

    return soundBuffer;
}

void ResourceManager::unloadSoundBuffer(std::string path)
{
    if (mGlobals->isDebug)
        path = "x64/Debug/data/" + path;
    else
        path = "data/" + path;

    if (mSoundBuffers.find(path) != mSoundBuffers.end())
    {
        mSoundBuffers.erase(path);
    }
}