#pragma once

#include<iostream>
#include<SDL2/SDL_ttf.h>

using namespace std;


enum TextSize{
  large = 0,
  small,
}; 

class TextMaster
{
	TTF_fonts *fonts[2];
	SDL_Renderer* gRenderer;

public:
	TextMaster(SDL_Renderer* Renderer){
		gRenderer  =Renderer;
		if( TTF_Init() == -1 )
        {
            cout<<"SDL_ttf could not initialize! SDL_ttf Error: "<<TTF_GetError()<<endl;
            exit(EXIT_FAILURE);
        }

		fonts[0] = TTF_OpenFont("./data/large.otf", 40);
		fonts[1] = TTF_OpenFont("./data/small.otf", 20);

		if (fonts[0] == 0 || fonts[1] == 0) {
			cout<<"Failed to load lazy font! SDL_ttf Error: "<< TTF_GetError()<<endl;
	        exit(EXIT_FAILURE);
		}
	}

	TTF_Font* getFont(int size) {
		if (s == 0) return fonts[0];

		if (s == 1) return fonts[1];

		return fonts[1];
	}

	void renderText(SDL_color textcolor, int size, string text, point &p){
		SDL_Surface* textSurface = TTF_RenderText_Solid(getFont(size), text.c_str(), textColor);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(render, textSurface);
		SDL_Rect inputCord = {0,0,textSurface->w, textSurface->h};
		SDL_Rect showCord = {p.x, p.y, 0, 0};
		SDL_RenderCopy(gRendere, textTexture, &inputCord, &showCord);
    	SDL_DestroyTexture(textTexture);
	}

	~TextMaster(){
		for (int i = 0; i < 2; i++) {
			TTF_CloseFont(fonts[i]);
			fonts[i] = NULL;
		}
		TTF_Quit();
	}
	
};