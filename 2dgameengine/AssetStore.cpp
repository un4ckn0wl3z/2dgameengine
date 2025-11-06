#include "AssetStore.h"
#include "Logger.h"

AssetStore::AssetStore() {
	Logger::Log("AssetStore constructor called!");
}

AssetStore::~AssetStore() {
	Logger::Log("AssetStore deconstructor called!");
}

void AssetStore::ClearAssets() {
	m_textures.clear();
}

void AssetStore::AddAssets(const std::string& assetId, const std::string filePath) {

}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId) const {
	return nullptr;
}
