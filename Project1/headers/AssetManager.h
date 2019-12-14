#pragma once
#include <map>
#include <SDL_mixer.h>
#include <SDL.h>

class AssetManager {

private:

	//Needed to make AssetManager a singleton class
	static AssetManager* sInstance;

	std::map<std::string, Mix_Music*> mMusic;
	//Used to cache all loaded sound effect files
	std::map<std::string, Mix_Chunk*> mSFX;

public:
	//-----------------------------------------
	//Returns a pointer to the class instance  
	//-----------------------------------------
	static AssetManager* Instance();
	//-----------------------------------------------------
	//Releases the class instance and sets it back to NULL 
	//-----------------------------------------------------
	static void Release();
	//------------------------------------------------------------------------------------------
	//If the music file was opened before, the cached Mix_Music* is returned
	//otherwise, the file is loaded and converted into a Mix_Music* and cached then returned
	//------------------------------------------------------------------------------------------
	Mix_Music* GetMusic(std::string filename);
	//------------------------------------------------------------------------------------------
	//If the SFX file was opened before, the cached Mix_Chunk* is returned
	//otherwise, the file is loaded and converted into a Mix_Chunk* and cached then returned
	//------------------------------------------------------------------------------------------
	Mix_Chunk* GetSFX(std::string filename);

private:
	AssetManager();
	~AssetManager();
};
