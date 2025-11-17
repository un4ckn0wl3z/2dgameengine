#pragma once
#include <iostream>
#include <map>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"

class AssetStore {
private:
	std::map<std::string, SDL_Texture*> m_textures;
	std::map<std::string, TTF_Font*> m_fonts;
	// TODO: fonts, autio
public:
	AssetStore();
	~AssetStore();

	void ClearAssets();

	void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string filePath);
	SDL_Texture* GetTexture(const std::string& assetId) ;

	void AddFont(
		const std::string& assetId, 
		const std::string filePath,
		int fontSize
		);
	TTF_Font* GetFont(const std::string& assetId);



};