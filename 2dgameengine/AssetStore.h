#pragma once
#include <iostream>
#include <map>
#include <string>
#include "SDL.h"

class AssetStore {
private:
	std::map<std::string, SDL_Texture*> m_textures;
	// TODO: fonts, autio
public:
	AssetStore();
	~AssetStore();

	void ClearAssets();
	void AddAssets(const std::string& assetId, const std::string filePath);
	SDL_Texture* GetTexture(const std::string& assetId) const;



};