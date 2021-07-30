#pragma once

#include <time.h>
#include <iostream>
#include <memory>
#include <iomanip>
#include "Man.hpp"
#include "SoundMaster.hpp"
#include "TextMaster.hpp"
#include "InputMaster.hpp"
#include "Coin.hpp"
#include "Virus.hpp"
#include "Map.hpp"
using namespace std;

namespace colors {
const SDL_Color red = {255, 0, 0, 255};
const SDL_Color blue = {0, 0, 255, 255};
const SDL_Color white = {255, 255, 255, 255};
const SDL_Color black = {0, 0, 0, 255};
const SDL_Color light_blue = {0, 102, 204, 255};
const SDL_Color light_brown = {255, 153, 51, 255};
const SDL_Color purple = {128, 0, 128, 255};
}

enum phase
{
	startup, pregame, play, paus, over
};

class CoviWar
{
	bool startMusic = true;

	SDL_Window* window;
	SDL_Renderer* renderer;
	int stage;
	mode M;
	phase currPhase;
	int counter;
	int refresh;

	ENetHost* server;
	ENetEvent event;
	ENetAddress address;
	ENetPeer* client;
	bool online;

	unique_ptr<ImageMaster> imageMaster;
	unique_ptr<SoundMaster> music;
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
	void paus_game();
	void over_game();

	void show_gamebar() {
		SDL_Texture* bar = imageMaster->get_image(11);
		SDL_Rect inputCord = {0, 0, 460, 120};
		// fill source for image
		const SDL_Rect showCord = {0, display::bar, 460, 120};
		SDL_RenderCopy(renderer, bar, &inputCord, &showCord);
		SDL_DestroyTexture(bar);
	}

	void show_money() {
		SDL_Texture* coin_texture = imageMaster->get_image(3);
		SDL_Rect inputCord = {0, 0, 20, 20};
		SDL_Rect showCord = {10, display::bar + 30, 20, 20};
		SDL_RenderCopy(renderer, coin_texture, &inputCord, &showCord);
		SDL_DestroyTexture(coin_texture);

		string coins = to_string(man1->get_money());
		point p = point{40, display::bar + 30};
		textMaster->renderText(colors::blue, 1, coins, p );

		SDL_Texture* man1_texture = imageMaster->get_image(14);
		inputCord = {0, 0, 18, 25};
		showCord = {105, display::bar + 30, 18, 25};
		SDL_RenderCopy(renderer, man1_texture, &inputCord, &showCord);
		SDL_DestroyTexture(man1_texture);

		string lives = to_string(man1->get_lives());
		point p3 = point{130, display::bar + 30};
		textMaster->renderText(colors::blue, 1, lives, p3);

		if (M == mode::single) {
			return;
		}

		coin_texture = imageMaster->get_image(3);
		inputCord = {0, 0, 20, 20};
		showCord = {10, display::bar + 70, 20, 20};
		SDL_RenderCopy(renderer, coin_texture, &inputCord, &showCord);
		SDL_DestroyTexture(coin_texture);

		coins = to_string(man2->get_money());
		p = point{40, display::bar + 70};
		textMaster->renderText(colors::blue, 1, coins, p );

		SDL_Texture* man2_texture = imageMaster->get_image(1);
		inputCord = {0, 0, 25, 25};
		showCord = {100, display::bar + 70, 25, 25};
		SDL_RenderCopy(renderer, man2_texture, &inputCord, &showCord);
		SDL_DestroyTexture(man2_texture);

		lives = to_string(man2->get_lives());
		p3 = point{130, display::bar + 70};
		textMaster->renderText(colors::blue, 1, lives, p3);
	}

	void show_sanitized() {
		if (man1->get_sanitized()) {
			SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
			if (stage)
			{
				SDL_Rect showCord = {160, display::bar + 35, man1->get_sanitized() / 3, 15};
				SDL_RenderFillRect(renderer, &showCord);
			}
			else{
				SDL_Rect showCord = {160, display::bar + 35, man1->get_sanitized() / 4, 15};
				SDL_RenderFillRect(renderer, &showCord);
			}
		}

		if (M == mode::single) {
			return;
		}

		if (man2->get_sanitized()) {
			SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
			if (stage)
			{
				SDL_Rect showCord = {160, display::bar + 75, man2->get_sanitized() / 3, 15};
				SDL_RenderFillRect(renderer, &showCord);
			}
			else{
				SDL_Rect showCord = {160, display::bar + 75, man2->get_sanitized() / 4, 15};
				SDL_RenderFillRect(renderer, &showCord);
			}
		}
	}

	void show_infected() {
		if (man1->get_infected()) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			SDL_Rect showCord = {260, display::bar + 35, man1->get_infected() / 13, 15};
			SDL_RenderFillRect(renderer, &showCord);
		}

		if (M == mode::single) {
			return;
		}

		if (man2->get_infected()) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			SDL_Rect showCord = {260, display::bar + 75, man2->get_infected() / 13, 15};
			SDL_RenderFillRect(renderer, &showCord);
		}
	}

	void show_masked() {
		if (man1->get_masked()) {
			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
			if (stage)
			{
				SDL_Rect showCord = {360, display::bar + 35, man1->get_masked() / 3, 15};
				SDL_RenderFillRect(renderer, &showCord);
			}
			else{
				SDL_Rect showCord = {360, display::bar + 35, man1->get_masked() / 5, 15};
				SDL_RenderFillRect(renderer, &showCord);
			}
		}

		if (M == mode::single) {
			return;
		}

		if (man2->get_masked()) {
			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
			if (stage)
			{
				SDL_Rect showCord = {360, display::bar + 75, man2->get_masked() / 3, 15};
				SDL_RenderFillRect(renderer, &showCord);
			}
			else{
				SDL_Rect showCord = {360, display::bar + 75, man2->get_masked() / 5, 15};
				SDL_RenderFillRect(renderer, &showCord);
			}			
		}
	}

	bool poll_event() {
		bool quit = false;
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
				return true;
			}
			else if (e.type == SDL_QUIT) {
				return true;
			}
			else {
				continue;
			}
		}
		return quit;
	}

	void manage_loop() {
		int curr;
		static int prev;
		int stall = (int) 1000.0 / display::fps;
		if (prev) {
			curr = SDL_GetTicks() - prev;
			if (curr < stall) {
				SDL_Delay(stall - curr);
			}
		}
		prev = SDL_GetTicks();
	}



public:
	CoviWar(): window(nullptr), renderer(nullptr), M(mode::single), currPhase(phase::startup), counter(0), refresh(0) {
		if (SDL_Init(SDL_INIT_EVERYTHING)) {
			cout << "Error" << SDL_GetError() << endl;
			exit(EXIT_FAILURE);
		}
		address = {0};
		address.host = ENET_HOST_ANY;
		address.port = 1244;
		server = enet_host_create(&address, 2, 2, 0, 0);
		if (server == NULL) {
	        cout<<"An error occurred while trying to create an ENet server host."<<endl;
	        return;
	    }
	    else{
	    	cout<<"Server created"<<endl;
	    }

		window = SDL_CreateWindow( "CoviWar Awareness", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, display::width, display::height, SDL_WINDOW_SHOWN);

		if (window == nullptr) {
			cout << "Error" << SDL_GetError() << endl;
			exit(EXIT_FAILURE);
		}

		renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
		if (renderer == nullptr )
		{
			cout << "Error" << SDL_GetError() << endl;
			exit(EXIT_FAILURE);
		}

		music = make_unique<SoundMaster>();
		inputMaster = make_unique<InputMaster>();
		// cout << "yes" << endl;

		imageMaster = make_unique<ImageMaster>(renderer);
		textMaster = make_unique<TextMaster>(renderer);
		// cout << "yes" << endl;
		map = make_unique<Map>(renderer, imageMaster.get());
		man1 = make_unique<Man>(inputMaster.get(), imageMaster.get(), 0);
		man2 = make_unique<Man>(inputMaster.get(), imageMaster.get(), 1);
		virus = make_unique<Virus>(imageMaster.get(), music.get());
		coin = make_unique<Coin>(music.get(), imageMaster.get());
		SDL_ShowCursor(SDL_DISABLE);
	};

	void run() {
		while (true) {
			// cout<<"running"<<endl;
			while (enet_host_service(server, &event, 0) > 0){
				if (event.type == ENET_EVENT_TYPE_CONNECT){
					online = true;
					client = event.peer;
					cout<<"Client connected"<<endl;
				}
				else if (event.type == ENET_EVENT_TYPE_RECEIVE){
					unsigned char* msg = event.packet->data;
					if (msg[0] == '1'){
						point p = {ByteToInt(&msg[1]), ByteToInt(&msg[5])};
						man2->set_pixel_val(p);
						if (msg[9] == '1')
						{
							man2->set_infected(true, ByteToInt(&msg[13]));
						}
						else{
							man2->set_infected(false, 0);
						}
						if (msg[17] == '1')
						{
							man2->set_masked(true, ByteToInt(&msg[21]));
						}
						else{
							man2->set_masked(false,0);
						}
						if (msg[25] == '1')
						{
							man2->set_sanitized(true, ByteToInt(&msg[29]));
						}
						else{
							man2->set_sanitized(false,0);
						}
						man2->set_money(ByteToInt(&msg[33]));
						man2->set_lives(ByteToInt(&msg[37]));
						man2->set_cord(ByteToInt(&msg[41]),ByteToInt(&msg[45]));
					}
					else if (msg[0] == '2'){
						if (currPhase == phase::play)
						{
							currPhase = phase::paus;
						}
						else if (currPhase == phase::paus)
						{
							currPhase = phase::play;
						}
					}
					else if (msg[0] == '4'){
						int id = ByteToInt(&msg[1]);
						virus->setState(id,0);
					}
					enet_packet_destroy(event.packet);
				}
				else if (event.type == ENET_EVENT_TYPE_DISCONNECT)
				{
					client->data = NULL;
					// event.peer->data = NULL;
				}
				else{}
			}

			inputMaster->update();
			if (startMusic) {
				Mix_PlayMusic(music->getMusic(), -1);
				startMusic = false;
			}
			if (currPhase == phase::startup) {startup_game();}
			else if (currPhase == phase::pregame) pregame_game();
			else if (currPhase == phase::play) play_game();
			else if (currPhase == phase::paus) paus_game();
			else if (currPhase == phase::over) over_game();

			if (poll_event()) {
				enet_host_destroy(server);
				return;
			}

			SDL_RenderPresent(renderer);
			manage_loop();
		}
	}

	~CoviWar() {
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	};

};