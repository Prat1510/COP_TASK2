#pragma once

#include <SDL2/SDL_mixer.h>
#include <memory>
#include <iostream>

class SoundMaster
{
	Mix_Music *music;

	// 0 = start; 1 = end; 2 = siren; 3 = enemy_death; 4 = chomp;
	vector<Mix_Chunk> *effects(5);

	bool loadMusic(string path) {
		bool success = true;

		music = Mix_LoadMUS(path);

		if (music == NULL) {
			cout << "Error: " << Mix_GetError() << endl;
			success = false;
		}

		return success;
	}

	bool laodEffects(string path, int index) {
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

		if ( !loadMusic("./data/luminousparticle.mp3") ) exit(EXIT_FAILURE);
		if ( !laodEffects("./data/pacman_beginning.wav", 0) ) exit(EXIT_FAILURE);
		if ( !laodEffects("./data/pacman_death.wav", 1) ) exit(EXIT_FAILURE);
		if ( !laodEffects("./data/66376e_Pacman_Siren_Sound_Effect.wav", 2) ) exit(EXIT_FAILURE);
		if ( !laodEffects("./data/gun-gird1.wav", 3) ) exit(EXIT_FAILURE);
		if ( !laodEffects("./data/pacman_chomp.wav", 4) ) exit(EXIT_FAILURE);

	}

	Mix_Music *getMusic() {
		return music;
	}

	Mix_Chunk *getEffects(string s) {
		if (s == "start") return effects[0];

		else if (s == "end") return effects[1];

		else if (s == "siren") return effects[2];

		else if (s == "enemy_death") return effects[3];

		else return effects[4];     //chomp
	}


	~SoundMaster() {
		Mix_HaltMusic();

		//free sound effects
		for (int i = 0; i <= 4; i++) {
			Mix_FreeChunk(effects[i]);
		}

		//Free the music
		Mix_FreeMusic( gMusic );

		Mix_Quit();
	}
};

