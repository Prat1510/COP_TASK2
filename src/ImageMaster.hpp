#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <map>
#include <string>
using namespace std;

// map<string, int> image_map =
// {
// 	{"player1_u", 0},
// 	{"player2_u", 1},
// 	{"mask_player1", 2},
// 	{"mask_player2", 3},
// 	{"coin", 4},
// 	{"virus_icon", 5},
// 	{"med", 6},
// 	{"sanitizer", 7},
// 	{"mask", 8},
// 	{"back1", 9},
// 	{"back2", 10},
// 	{"bar", 11},
// 	{"player1_d", 12},
// 	{"player2_d", 13},
// 	{"player1_l", 14},
// 	{"player2_l", 15},
// 	{"player1_r", 16},
// 	{"player2_r", 17},
// };


class ImageMaster
{
	vector<SDL_Surface*> Images;
	SDL_Renderer* gRenderer;

	void loadPng(const char *path, int image_num)
	{
		// cout << "yes" << endl;
		// SDL_Texture* newTexture = NULL;
		SDL_Surface* loadedSurface = IMG_Load( path );
		// cout << "yes" << endl;
		if ( loadedSurface == NULL )
		{
			cout << "Unable to load image " << "! SDL Error: " << SDL_GetError() << endl;
		}
		// else
		// {
		// 	newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		// 	if ( newTexture == NULL )
		// 	{
		// 		cout << "Unable to create texture from " << "! SDL Error: " << SDL_GetError() << endl;
		// 	}
		// 	// SDL_FreeSurface( loadedSurface );
		// }
		Images.push_back(loadedSurface);
	}

public:
	ImageMaster(SDL_Renderer* renderer)
	{
		// cout << "yes" << endl;

		gRenderer = renderer;
		int imgFlags = IMG_INIT_PNG;

		if ( !( IMG_Init( imgFlags ) & imgFlags ) )
		{
			cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
			exit(EXIT_FAILURE);
		}

		// cout << "yes" << endl;

		for (int i = 0; i < 18; i++)
		{
			string s = "./data/" + to_string(i) + ".png";
			const char *path = s.c_str();
			// cout << s << endl;
			loadPng(path, i);
			// cout << s << endl;
		}
		// cout << "yes" << endl;

	}

	SDL_Texture* get_image(int index)
	{
		return SDL_CreateTextureFromSurface( gRenderer,Images[index]);
	}

	void render_image(SDL_Texture &texture, SDL_Rect &src, SDL_Rect &dst)
	{
		SDL_RenderCopy(gRenderer, &texture, &src, &dst);
	}

	~ImageMaster()
	{
		IMG_Quit();
	}
};
