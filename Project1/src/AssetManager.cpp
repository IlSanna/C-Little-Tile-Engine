#include "../headers/AssetManager.h"
#include <iostream>
#include <string>
//Initializing to NULL
AssetManager* AssetManager::sInstance = NULL;

AssetManager* AssetManager::Instance() {

	//Create a new instance if no instance was created before
	if(sInstance == NULL)
		sInstance = new AssetManager();

	return sInstance;
}

void AssetManager::Release() {

	delete sInstance;
	sInstance = NULL;
}

AssetManager::AssetManager() {

}

AssetManager::~AssetManager() {
	//Freeing all loaded music
	for(auto music : mMusic) {

		if(music.second != NULL) {

			Mix_FreeMusic(music.second);
		}
	}

	mMusic.clear();

	//Freeing all loaded sound effects
	for(auto sfx : mSFX) {

		if(sfx.second != NULL) {

			Mix_FreeChunk(sfx.second);
		}
	}

	mSFX.clear();
}

Mix_Music* AssetManager::GetMusic(std::string filename) {

	//Get the full path of the WAV file
	//std::string fullPath = SDL_GetBasePath();
	//fullPath.append("content\music\"" + filename);

	//If the file has not been already loaded, load it and add it to the mMusic map
	if(mMusic[filename] == nullptr) {

		mMusic[filename] = Mix_LoadMUS(filename.c_str());
		//Error handling for file loading
		if(mMusic[filename] == NULL)
			printf("Music Loading Error: File-%s Error-%s", filename.c_str(), Mix_GetError());
	}

	//returning the cached file from the map
	return mMusic[filename];
}

Mix_Chunk* AssetManager::GetSFX(std::string filename) {

	//Get the full path of the WAV file
	std::string fullPath = SDL_GetBasePath();
	fullPath.append("Assets/" + filename);

	//If the file has not been already loaded, load it and add it to the mSFX map
	if(mSFX[filename] == nullptr) {

		mSFX[filename] = Mix_LoadWAV(filename.c_str());
		//Error handling for file loading
		if(mSFX[filename] == NULL)
			printf("SFX Loading Error: File-%s Error-%s", filename.c_str(), Mix_GetError());
	}

	//returning the cached file from the map
	return mSFX[filename];
}
