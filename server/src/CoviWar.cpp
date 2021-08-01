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
#include "CoviWar.hpp"
#include "Basic.hpp"
#include <SDL2/SDL_mixer.h>
#include "enet.h"
using namespace std;

void CoviWar::startup_game() {
	SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
	SDL_RenderClear(renderer);

	if (counter == 0) {
		point p1 =  point{115, 200};

		textMaster->renderText(colors::black, TextSize::large, "C o v i - W A R", p1);

		if (++refresh < 40) {
			point p2 = point{85, 360};
			textMaster->renderText(colors::red, TextSize::small, "PRESS ENTER TO CONTINUE", p2);
		}
		else if (refresh < 80) {refresh++;}
		else {refresh = 0;}

		if (inputMaster->is_edge_key(0, 6) || inputMaster->is_edge_key(1, 6))
		{
			Mix_PlayChannel(5, music->getEffects("stage"), 0);
			refresh = 0;
			counter++;
		}
	}
	else if (counter == 1) {
		if (++refresh < 40) {
			point p1 = point{120, 140};
			textMaster->renderText(colors::purple, TextSize::large, "SELECT MODE", p1);
		}
		else if (refresh < 80) {refresh++;}
		else {refresh = 0;}

		if (M == mode::single) {
			const SDL_Rect rect1 =  {180, 295, 120, 40};
			SDL_SetRenderDrawColor(renderer, 255, 153, 51, 255);
			SDL_RenderFillRect(renderer, &rect1);
			point p2 = point{200, 303};
			point p3 = point{210, 363};
			textMaster->renderText(colors::light_blue, TextSize::small, "SINGLE", p2);
			textMaster->renderText(colors::light_brown, TextSize::small, "DUAL", p3);
		}
		else {
			const SDL_Rect rect2 =  {180, 355, 120, 40};
			SDL_SetRenderDrawColor(renderer, 255, 153, 51, 255);
			SDL_RenderFillRect(renderer, &rect2);
			point p2 = point{200, 303};
			point p3 = point{210, 363};
			textMaster->renderText(colors::light_brown, TextSize::small, "SINGLE", p2);
			textMaster->renderText(colors::light_blue, TextSize::small, "DUAL", p3);
		}

		if (inputMaster->is_edge_key(0, 6) || inputMaster->is_edge_key(1, 6))
		{
			Mix_PlayChannel(5, music->getEffects("stage"), 0);
			counter++;
		}
		if (inputMaster->is_edge_key(0, 1) || inputMaster->is_edge_key(1, 1))
		{
			M = mode::dual;
		}
		else if (inputMaster->is_edge_key(0, 0) || inputMaster->is_edge_key(1, 0)) {
			M = mode::single;
		}
	}
	else if (counter == 2) {
		// initialising variables
		currPhase = phase::pregame;
		counter = 0;
		stage = 0;
		refresh = 0;

		srand((unsigned int)time(nullptr));
		map->initialise(M, client, online);
		// cout<<"here"<<end;
		coin->init(*map);
		virus->init(*map);

		// man1
		man1->init();
		man1->set_lives(3);
		man1->set_money(0);
		man1->set_masked(false, 0);
		man1->set_sanitized(false, 0);
		man1->set_infected(false, 0);

		//
		if (M == mode::dual)
		{
			// if (online){}
			// else{
			man2->init();
			man2->set_lives(3);
			man2->set_money(0);
			man2->set_masked(false, 0);
			man2->set_sanitized(false, 0);
			man2->set_infected(false, 0);
		}
	}
	// play music
}

void CoviWar::pregame_game() {
	map->show(stage);
	show_gamebar();
	man1->show(M);
	if (M == mode::dual)
	{
		man2->show(M);
	}

	if (counter == 0) {
		refresh++;
		if (refresh < 180) {
			point p1 = point{140, 150};
			point p2 = point{90, 80};
			textMaster->renderText(colors::white, TextSize::large, "STAGE : " + to_string(stage + 1) , p1);
			textMaster->renderText(colors::white, TextSize::small, "LET'S CONQUER COVID-19", p2);
		}
		else {
			currPhase = phase::play;
			refresh = 0;
			Mix_PlayChannel(0, music->getEffects("start"), 0);
		}
	}
}

void CoviWar::play_game() {
	t += 1.25 / 60.0;
	// cout << t << endl;
	if (t > 20) {
		t = 0;
		if (stage == 1) virus->regenerate();
	}

	map->show(stage);
	show_gamebar();
	show_money();
	show_sanitized();
	show_infected();
	show_masked();
	man1->show(M);
	if (M == mode::dual)
	{
		man2->show(M);
	}
	coin->show();
	virus->show();
	if (online && inputMaster->is_edge_key(0, 7)) {
		currPhase = paus;
		counter = 0;
		refresh = 0;
		unsigned char msg[1];
		msg[0] = '2';
		ENetPacket* packet = enet_packet_create(msg, 2, 0);
		enet_peer_send(client, 0 , packet);
	}
	else if (inputMaster->is_edge_key(0, 7) || inputMaster->is_edge_key(1, 7)) {
		currPhase = phase::paus;
		refresh = 0;
		counter = 0;
	}

	if (counter == 0) {
		man1->move(M, *map, client, online);
		if (man1->get_sanitized() > 0) {
			man1->set_sanitized(true, man1->get_sanitized() - 1);
		}
		else {
			man1->set_sanitized(false, 0);
		}

		if (man1->get_masked() > 0) {
			man1->set_masked(true, man1->get_masked() - 1);
		}
		else {
			man1->set_masked(false, 0);
		}

		if (man1->is_infected()) {
			if (man1->get_infected()) {
				man1->set_infected(true, man1->get_infected() - 1);
			}
			else {
				man1->set_lives(man1->get_lives() - 1);
				Mix_PlayChannel(1, music->getEffects("end"), 0);
				man1->set_infected(false, 0);
				man1->init();
			}
		}

		if (M == mode::dual) {
			if (online)
			{
			}
			else {
				man2->move(M, *map, client, online);
				if (man2->get_sanitized() > 0) {
					man2->set_sanitized(true, man2->get_sanitized() - 1);
				}
				else {
					man2->set_sanitized(false, 0);
				}

				if (man2->get_masked() > 0) {
					man2->set_masked(true, man2->get_masked() - 1);
				}
				else {
					man2->set_masked(false, 0);
				}

				if (man2->is_infected()) {
					if (man2->get_infected()) {
						man2->set_infected(true, man2->get_infected() - 1);
					}
					else {
						man2->set_lives(man2->get_lives() - 1);
						Mix_PlayChannel(1, music->getEffects("end"), 0);
						man2->set_infected(false, 0);
						man2->init();
						counter = 0;
					}
				}
			}
		}

		if (M == mode :: single) {
			if (man1->get_lives() == 0) {
				Mix_PlayChannel(1, music->getEffects("end"), 0);
				currPhase = phase::over;
				counter = 0;
			}
		}
		else {
			if (man1->get_lives() == 0 || man2->get_lives() == 0) {
				Mix_PlayChannel(1, music->getEffects("end"), 0);
				man1->move(M, *map, client, online);
				currPhase = phase::over;
				counter = 0;
			}

			// if (man1->get_lives() == 0 && man1->get_money() < man2->get_money()) {
			// 	currPhase = phase::over;
			// 	counter = 0;
			// }

			// if (man2->get_lives() == 0 && man1->get_money() > man2->get_money()) {
			// 	currPhase = phase::over;
			// 	counter = 0;
			// }
		}

		virus->move(stage, *map, online, client );
		virus->virus_catch_man(M, *man1);
		virus->virus_catch_man(M, *man2);

		if (coin->findState(*man1, *man2, M, stage, online)) {
			counter++;
		}
	}
	else if (counter == 1) {
		if (stage >= 1) {                        //both stages are over
			currPhase = phase::over;
			counter = 0;
		}
		else if (stage == 0) {                   //play second stage
			srand((unsigned int)time(nullptr));
			map->initialise(M, client, online);

			coin->init(*map);
			virus->init(*map);

			man1->init();
			man1->set_masked(false, 0);
			man1->set_sanitized(false, 0);
			man1->set_infected(false, 0);

			if (M == mode::dual && !online)
			{
				man2->init();
				man2->set_masked(false, 0);
				man2->set_sanitized(false, 0);
				man2->set_infected(false, 0);
			}

			counter = 0;
			currPhase = phase::pregame;
			counter = 0;
			stage = 1;
			refresh = 0;
			t = 0;

		}
	}
}


void CoviWar::paus_game() {
	coin->show();
	virus->show();
	map->show(stage);
	man1->show(M);
	man2->show(M);
	show_gamebar();
	show_infected();
	show_infected();
	show_masked();
	if (online && inputMaster->is_edge_key(0, 7)) {
		currPhase = play;
		counter = 0;
		refresh = 0;
		unsigned char msg[1];
		msg[0] = '2';
		ENetPacket* packet = enet_packet_create(msg, 2, 0);
		enet_peer_send(client, 0 , packet);
	}
	else if (inputMaster->is_edge_key(0, 7) || inputMaster->is_edge_key(1, 7))
	{
		currPhase = play;
		counter = 0;
		refresh = 0;
	}

	if (refresh < 30)
	{
		point p1 = point{140, 200};
		textMaster->renderText(colors::white, TextSize::large, "P A U S E", p1);
		refresh++;
	}
	else if (refresh < 60)
	{
		refresh++;
	}
	else {
		refresh = 0;
	}

}

void CoviWar::over_game() {

	SDL_SetRenderDrawColor(renderer, 0, 102, 204, 255);
	SDL_RenderClear(renderer);

	if (M == mode::single)
	{
		if (man1->get_lives() == 0) {
			point p1 = point{150, 100};
			point p2 = point{95, 150};
			point p3 = point{135 , 220};
			textMaster->renderText(colors::light_brown, TextSize::large, "OOPS!!!", p1);
			textMaster->renderText(colors::light_brown, TextSize::small, "BETTER LUCK NEXT TIME", p2);
			textMaster->renderText(colors::light_brown, TextSize::small, "SCORE IS : " + to_string(man1->get_money()), p3);
		}
		else {
			point p1 = point{135, 100};
			point p2 = point{100, 150};
			point p3 = point{135 , 220};
			textMaster->renderText(colors::light_brown, TextSize::large, "HOORAY!!!", p1);
			textMaster->renderText(colors::light_brown, TextSize::small, "U CONQUERED COVID-19", p2);
			textMaster->renderText(colors::light_brown, TextSize::small, "SCORE IS : " + to_string(man1->get_money()), p3);
		}

		point p3 = point{80, 400};
		if (refresh < 30) {textMaster->renderText(colors::white, TextSize::small, "PRESS ENTER TO CONTINUE", p3); refresh++;}
		else if (refresh < 60) {refresh++;}
		else {refresh = 0;}

		if (inputMaster->is_pressed_key(0, 6))
		{
			currPhase = startup;
			counter = 0;
			refresh = 0;
		}
	}
	else {
		if (man1->get_money() > man2->get_money()) {
			point p2 = point{95, 130};
			point p3 = point{95, 230};
			point p4 = point{95, 280};
			textMaster->renderText(colors::white, TextSize::large, "PLAYER 1 WON", p2);
			textMaster->renderText(colors::light_brown, TextSize::small, "PLAYER - 1 SCORE : " + to_string(man1->get_money()), p3);
			textMaster->renderText(colors::light_brown, TextSize::small, "PLAYER - 2 SCORE : " + to_string(man2->get_money()), p4);
		}
		else if (man1->get_money() < man2->get_money())
		{
			point p2 = point{95, 130};
			point p3 = point{95, 230};
			point p4 = point{95, 280};
			textMaster->renderText(colors::white, TextSize::large, "PLAYER 2 WON", p2);
			textMaster->renderText(colors::light_brown, TextSize::small, "PLAYER - 2 SCORE : " + to_string(man2->get_money()), p3);
			textMaster->renderText(colors::light_brown, TextSize::small, "PLAYER - 1 SCORE : " + to_string(man1->get_money()), p4);
		}
		else {
			point p2 = point{95, 130};
			point p3 = point{95, 230};
			point p4 = point{95, 280};
			textMaster->renderText(colors::white, TextSize::large, "IT'S A DRAW", p2);
			textMaster->renderText(colors::light_brown, TextSize::small, "PLAYER - 1 SCORE : " + to_string(man1->get_money()), p3);
			textMaster->renderText(colors::light_brown, TextSize::small, "PLAYER - 2 SCORE : " + to_string(man2->get_money()), p4);
		}

		point p1 = point{80, 400};
		if (refresh < 30) {textMaster->renderText(colors::white, TextSize::small, "PRESS ENTER TO CONTINUE", p1); refresh++;}
		else if (refresh < 60) {refresh++;}
		else {refresh = 0;}

		if (inputMaster->is_pressed_key(0, 6) || inputMaster->is_pressed_key(1, 6))
		{
			currPhase = startup;
			counter = 0;
			refresh = 0;
		}
	}
}




