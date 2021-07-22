#pragma once;

#include <time.h>
#include <iostrean>
#include <memory>
#include <iomanip>
#include "Man.hpp"
#include "SoundMaster.hpp"
#include "TextMaster.hpp"
#include "InputMaster.hpp "
#include "Coin.hpp"
#include "Virus.hpp"
#include "Map.hpp"
using namespace std;

namespace colors{
	const SDL_Color red = {255,0,0,255};
	const SDL_Color blue = {0,0,255,255};
	const SDL_Color white = {255,255,255,255};
	const SDL_Color black = {0,0,0,255};
}

enum phase
{
	startup, pregame, play, pause, over	
};

class CoviWar
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	int stage;
	mode M;
	phase currPhase;

	unique_ptr<ImageMaster> imageMaster;
	unique_ptr<SoundMaster> soundMaster;
	unique_ptr<InputMaster> inputMaster;
	unique_ptr<Map> map;
	unique_ptr<Coin> coin;
	unique_ptr<Virus> virus;
	unique_ptr<Man> man1;
	unique_ptr<Man> man2;
	unique_ptr<TextMaster> textMaster;

	void startup_game();
	void pregame_game();
	void play_game();	
	void pause_game();
	void over_game();

	void show_gamebar(){
		SDL_Texture* bar = imageMaster->get_image(10);
		SDL_Rect inputCord = {};
		// fill source for image
		SDL_Rect showCord = {0, display::bar, display::x, display::y - display::bar};
		SDL_RenderCopy(renderer, bar, inputCord, showCord);
		SDL_DestroyTexture(bar);
	}

	void show_score(){
		string coins = "COINS -- > " + to_string(man1->get_money());
		textMaster->renderText(colors::blue, int size, point{30, display::bar + 30}, coins);
		
		SDL_Texture* man1_texture = imageMaster->get_image(0);
		SDL_Rect inputCord = {0,0};
		SDL_Rect showCord = {60, display::bar + 30};
		SDL_QueryTexture(man1_texture, nullptr, nullptr, &inputCord.w, &inputCord.h);
		SDL_QueryTexture(man1_texture, nullptr, nullptr, &showCord.w, &showCord.h);
		SDL_RenderCopy(renderer, man1_texture, &inputCord, &showCord);
		SDL_DestroyTexture(man1_texture);

		string lives = "Lives :" + to_string(man1->get_lives());
		textMaster->renderText(colors::blue, int size, point{100, display::bar + 30}, lives);

		if (M = mode::single){
			return;
		}

		string coins = "COINS -- > " + to_string(man2->get_money());
		textMaster->renderText(colors::red, int size, point{30, display::bar + 60}, coins);
		
		SDL_Texture* man1_texture = imageMaster->get_image(1);
		SDL_Rect inputCord = {0,0};
		SDL_Rect showCord = {60, display::bar + 60};
		SDL_QueryTexture(man1_texture, nullptr, nullptr, &inputCord.w, &inputCord.h);
		SDL_QueryTexture(man1_texture, nullptr, nullptr, &showCord.w, &showCord.h);
		SDL_RenderCopy(renderer, man1_texture, &inputCord, &showCord);
		SDL_DestroyTexture(man1_texture);

		string lives = "Lives :" + to_string(man1->get_lives());
		textMaster->renderText(colors::red, int size, point{100, display::bar + 60}, lives);
	}

	void show_sanitized(){
		if (man1->get_sanitized()){
			SDL_SetRenderDrawColor(renderer, 0,255,255,255);
			SDL_Rect showCord = {160, display::bar + 30, man1->get_sanitized()/4, 15};
			SDL_RenderFillRect(renderer, &showCord);
		}

		if (M = mode::single){
			return;
		}

		if (man2->get_sanitized()){
			SDL_SetRenderDrawColor(renderer, 0,255,255,255);
			SDL_Rect showCord = {160, display::bar + 60, man2->get_sanitized()/4, 15};
			SDL_RenderFillRect(renderer, &showCord);
		}
	}

	void show_infected(){
		if (man1->get_sanitized()){
			SDL_SetRenderDrawColor(renderer, 255,255,0,255);
			SDL_Rect showCord = {260, display::bar + 30, man1->get_sanitized()/4, 15};
			SDL_RenderFillRect(renderer, &showCord);
		}

		if (M = mode::single){
			return;
		}

		if (man2->get_sanitized()){
			SDL_SetRenderDrawColor(renderer,255,255,0,255);
			SDL_Rect showCord = {260, display::bar + 60, man2->get_sanitized()/4, 15};
			SDL_RenderFillRect(renderer, &showCord);
		}
	}

	void show_masked(){
		if (man1->get_sanitized()){
			SDL_SetRenderDrawColor(renderer,255,0,255,255);
			SDL_Rect showCord = {360, display::bar + 30, man1->get_sanitized()/4, 15};
			SDL_RenderFillRect(renderer, &showCord);
		}

		if (M = mode::single){
			return;
		}

		if (man2->get_sanitized()){
			SDL_SetRenderDrawColor(renderer,255,0,255,255);
			SDL_Rect showCord = {360, display::bar + 60, man2->get_sanitized()/4, 15};
			SDL_RenderFillRect(renderer, &showCord);
		}
	}

	bool poll_event(){
		bool quit = false;
		SDL_Event e;
		while (SDL_PollEvent(&e)!=0){
			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){
				return true;
			}
			else if(e.type == SDL_QUIT){
				return true;
			}
			else{
				continue;
			} 
		}
    	return quit;
	}

	void manage_loop(){
		int curr;
		static int prev;
		int stall = (int) 1000.0/display::fps;
		if (prev){
			curr = SDL_GetTicks() - prev;
			if (curr<stall){
				SDL_Delay(stall - curr);
			}
		}
		prev = SDL_GetTicks();
	}



public:
	CoviWar():window(nullptr), renderer(nullptr), currPhase(phase::startup), M(mode::single){

		if (SDL_Init(SDL_INIT_EVERYTHING)){
			cout<<"Error"<<SDL_GetError()<<endl;
			exit(EXIT_FAILURE);
		}
		window = SDL_CreateWindow( "CoviWar Awareness", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, display::x, display::y, SDL_WINDOW_SHOWN);
		
		if (window == nullptr){
			cout<<"Error"<<SDL_GetError()<<endl;
			exit(EXIT_FAILURE);
		}

		renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
        if(renderer == nullptr )
        {
            cout<<"Error"<<SDL_GetError()<<endl;
			exit(EXIT_FAILURE);
        }

        soundMaster = make_unique<SoundMaster>();
        inputMaster = make_unique<InputMaster>();
        imageMaster = make_unique<ImageMaster>(renderer);
        man1 = make_unique<Man>(inputMaster.get(), imageMaster.get(), 0);
        man2 = make_unique<Man>(inputMaster.get(), imageMaster.get(), 1);
        virus = make_unique<Virus>(imageMaster.get(), soundMaster.get());
        coin = make_unique<Coin>(soundMaster.get(), imageMaster.get());
        SDL_ShowCursor(SDL_DISABLE);
	};

	void run(){
		while (true){
			inputMaster->update();
			if (currPhase == phase::startup) startup_game();
			else if (currPhase == phase::pregame) pregame_game();
			else if (currPhase == phase::play) play_game();
			else if (currPhase == phase::pause) pause_game();
			else if (currPhase == phase::over) over_game();

			if (poll_event()){}
			else{return;}

			SDL_RenderPresent(renderer);
			manage_loop(); 
		}
	}

	~CoviWar(){
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
    	SDL_Quit();
	};
	
};