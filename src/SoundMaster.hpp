#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <memory>
#include <iostream>
#include <string>
using namespace std;

class SoundMaster
{
	Mix_Music *music;
	Mix_Chunk *effects[7];

	bool loadMusic(const char *path) {
		bool success = true;

		music = Mix_LoadMUS(path);

		if (music == NULL) {
			cout << "Error: " << Mix_GetError() << endl;
			success = false;
		}

		return success;
	}

	bool loadEffects(const char *path, int index) {
		bool success = true;

		effects[index] = Mix_LoadWAV(path);

		if (effects[index] == NULL) {
			cout << "Error: " << Mix_GetError() << endl;
			success = false;
		}

		return success;
	}

public:
	SoundMaster() {
		if ( SDL_Init( SDL_INIT_AUDIO ) < 0 ) {
			cout << "Error: " << SDL_GetError() << endl;
			exit(EXIT_FAILURE);
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
			cout << "Error: " << Mix_GetError() << endl;
			exit(EXIT_FAILURE);
		}

		if ( !loadMusic("./data/music.wav") ) {
			exit(EXIT_FAILURE);
		}
		if ( !loadEffects("./data/start.wav", 0) ) {
			exit(EXIT_FAILURE);
		}
		if ( !loadEffects("./data/end.wav", 1) ) {
			exit(EXIT_FAILURE);
		}
		if ( !loadEffects("./data/coin.wav", 2) ) {
			exit(EXIT_FAILURE);
		}
		if ( !loadEffects("./data/mask.wav", 3) ) {
			exit(EXIT_FAILURE);
		}
		if ( !loadEffects("./data/med.wav", 4) ) {
			exit(EXIT_FAILURE);
		}
		if ( !loadEffects("./data/stage.wav", 5) ) {
			exit(EXIT_FAILURE);
		}
		if ( !loadEffects("./data/virus.wav", 6) ) {
			exit(EXIT_FAILURE);
		}

	}

	Mix_Music *getMusic() {
		return music;
	}

	Mix_Chunk *getEffects(string s) {
		if (s == "start") return effects[0];

		else if (s == "end") return effects[1];

		else if (s == "coin") return effects[2];

		else if (s == "mask") return effects[3];

		else if (s == "stage") return effects[4];

		else if (s == "med") return effects[5];

		else return effects[6];     //chomp
	}


	~SoundMaster() {
		Mix_HaltMusic();

		//free sound effects
		for (int i = 0; i <= 4; i++) {
			Mix_FreeChunk(effects[i]);
		}
		// Mix_FreeChunk( start );
		// Mix_FreeChunk( end );
		// Mix_FreeChunk( siren );
		// Mix_FreeChunk( enemy_death );
		// Mix_FreeChunk( chomp );

		//Free the music
		Mix_FreeMusic( music );

		Mix_Quit();
	}
};