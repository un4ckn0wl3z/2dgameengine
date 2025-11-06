#include "AssetStore.h"
#include "Logger.h"
#include "SDL_image.h"
#include "SDL.h"

AssetStore::AssetStore() {
	Logger::Log("AssetStore constructor called!");
}

AssetStore::~AssetStore() {
	ClearAssets();
	Logger::Log("AssetStore deconstructor called!");
}

void AssetStore::ClearAssets() {
	for (auto texture : m_textures) {
		SDL_DestroyTexture(texture.second);
	}
	m_textures.clear();
	Logger::Log("AssetStore assets cleared!");
}

void AssetStore::AddAssets(SDL_Renderer* renderer, const std::string& assetId, const std::string filePath) {
	SDL_Surface* surface = IMG_Load(filePath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	m_textures.emplace(assetId, texture);
}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId)  {
	return m_textures[assetId];
}
