#pragma once
#include "SparkGlobals.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <unordered_map>
#include <string>
#include <iostream>
#include <memory>

class ResourceManager
{
public:
	ResourceManager(std::shared_ptr<SparkGlobals> globals);
	~ResourceManager();

	std::shared_ptr<sf::Texture> getTexture(std::string path);
	void unloadTexture(std::string path);

	std::shared_ptr<sf::Font> getFont(std::string path);
	void unloadFont(std::string path);

	std::shared_ptr<sf::SoundBuffer> getSoundBuffer(std::string path);
	void unloadSoundBuffer(std::string path);

private:
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> mTextures;
	std::unordered_map<std::string, std::shared_ptr<sf::Font>> mFonts;
	std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> mSoundBuffers;

	std::shared_ptr<SparkGlobals> mGlobals;
};

