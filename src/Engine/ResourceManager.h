#pragma once
#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <string>
#include <iostream>
#include <memory>

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	std::shared_ptr<sf::Texture> getTexture(std::string path);
	void unloadTexture(std::string path);

private:
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> mTextures;
};

