#pragma once

#include<iostream>
#include <SDL2/SDL_ttf.h>
using namespace std;


//larger font in index 0 and smaller in index1

class fontManager {
	TTF_fonts *fonts[2];

public:

	fontManager() {
		if (TTF_Init() < 0) {
			cout << "Error: " << TTF_GetError() << endl;
			exit(EXIT_FAILURE);
		}

		fonts[0] = TTF_OpenFont("./data/large.otf", 36);
		fonts[1] = TTF_OpenFont("./data/small.otf", 16);

		if (fonts[0] == 0 || fonts[1] == 0) {
			cout << "Error: " << TTF_GetError() << endl;
			exit(EXIT_FAILURE);
		}
	}

	TTF_Font *getFont(string s) {
		if (s == "large") return fonts[0];

		if (s == "small") return fonts[1];

		return fonts[1];
	}


	~fontManager() {
		for (int i = 0; i < 2; i++) {
			TTF_CloseFont(fonts[i]);
		}
		TTF_Quit();
	}
}


