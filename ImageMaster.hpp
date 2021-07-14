#pragma once 

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <map>
#include <string>
using namespace std;

map<string, int> image_map = 
{
	{"player1", 0}, 
	{"player2", 1}, 
	{"mask_player1", 2}, 
	{"mask_player2", 3}, 
	{"coin", 4}, 
	{"virus", 5}, 
	{"med", 6}, 
	{"sanitizer", 7}, 
	{"mask", 8},
	{"back1", 9},
	{"back2", 10},
	{"back3", 11}
};


class Image_Master
{
	vector<SDL_Texture*> Images;
	SDL_Renderer* gRenderer;

	void loadPng(std::string* loacation, int image_num)
	{
		SDL_Texture* newTexture = NULL;
		SDL_Surface* loadedSurface = IMG_Load( path.c_str() );	

		if( loadedSurface == NULL )
		{
			cout<<"Unable to load image " << path.c_str() << "! SDL Error: " << SDL_GetError() <<endl;
		}
		else
		{
			newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
			if( newTexture == NULL )
			{
				cout<<"Unable to create texture from "<<path.c_str()<<"! SDL Error: "<<SDL_GetError()<<endl;
			}
			SDL_FreeSurface( loadedSurface );
		}
		Images[image_num] = newTexture;
	}

	public:
		Image_Master(SDL_Renderer* renderer)
		{
			gRenderer = renderer;
			int imgFlags = IMG_INIT_PNG;
	
			if( !( IMG_Init( imgFlags ) & imgFlags ) )
	    	{
	        	cout<< "SDL_image could not initialize! SDL_image Error: "<< IMG_GetError()<< endl;
	        	exit(EXIT_FAILURE);
	      	}

	      	for (auto const &pair: m)
	      	{
	      		loadPng(pair.first + ".png", pair.second);
	      	}
	    }

		SDL_Texture* get_image(int index)
		{
			return Images[index];
		}

		void render_image(SDL_Texture &texture, SDL_Rect &src, SDL_Rect &dst)
		{
			SDL_RenderCopy(gRenderer, &texture, &src, &dst);	
		}

		~ImageMaster() 
		{
			IMG_Quit(); 
		}
}