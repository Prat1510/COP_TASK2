#pragma once
#include "enet.h"
#include <SDL2/SDL_mixer.h>
#include "Basic.hpp"
#include "ImageMaster.hpp"
#include "Map.hpp"
#include "Virus.hpp"
#include "Man.hpp"
#include "SoundMaster.hpp"

enum class edible {
	coin = 0,
	medicine,
	mask,
	sanitizer,
	nothing,
};


class Coin
{
	edible allStates[block::y][block::x];
	SoundMaster *music;
	ImageMaster *imageMaster;

public:
	Coin(SoundMaster *music_, ImageMaster *imageMaster_) : music(music_), imageMaster(imageMaster_) {}

	void init(Map &map) {
		for (int i = 0; i < block::y; i++) {
			for (int j = 0; j < block::x; j++) {
				point temp = {j, i};
				if (map.FindState(temp) == Element_Map::coin) {
					allStates[i][j] = edible::coin;
				}
				else if (map.FindState(temp) == Element_Map::med) {
					allStates[i][j] = edible::medicine;
				}
				else if (map.FindState(temp) == Element_Map::mask) {
					allStates[i][j] = edible::mask;
				}
				else if (map.FindState(temp) == Element_Map::sanitizer) {
					allStates[i][j] = edible::sanitizer;
				}
				else {
					allStates[i][j] = edible::nothing;
				}
			}
		}

		return;
	}

	void show() {
		SDL_Texture *coin_texture = imageMaster->get_image(2);
		SDL_Texture *medicine_texture = imageMaster->get_image(6);
		SDL_Texture *mask_texture = imageMaster->get_image(8);
		SDL_Texture *sanitizer_texture = imageMaster->get_image(7);

		int sz = block::size;
		for (int i = 0; i < block::y; i++) {
			for (int j = 0; j < block::x; j++) {

				if (allStates[i][j] == edible::coin) {
					SDL_Rect rect1 = {0, 0, sz / 4, sz / 4};
					SDL_Rect rect2 = {j * sz + 7, i * sz + 7, sz / 4, sz / 4};
					imageMaster->render_image(*coin_texture, rect1, rect2);
				}
				else if (allStates[i][j] == edible::medicine) {
					SDL_Rect rect1 = {0, 0, sz, sz};
					SDL_Rect rect2 = {j * sz, i * sz, sz, sz};
					imageMaster->render_image(*medicine_texture, rect1, rect2);
				}
				else if (allStates[i][j] == edible::mask) {
					SDL_Rect rect1 = {0, 0, sz, sz};
					SDL_Rect rect2 = {j * sz, i * sz, sz, sz};
					imageMaster->render_image(*mask_texture, rect1, rect2);
				}
				else if (allStates[i][j] == edible::sanitizer) {
					SDL_Rect rect1 = {0, 0, sz, sz};
					SDL_Rect rect2 = {j * sz, i * sz, sz, sz};
					imageMaster->render_image(*sanitizer_texture, rect1, rect2);
				}
			}
		}

		SDL_DestroyTexture(coin_texture);
		SDL_DestroyTexture(medicine_texture);
		SDL_DestroyTexture(mask_texture);
		SDL_DestroyTexture(sanitizer_texture);

		return;
	}


	bool findState (Man &m1, Man &m2, mode M, int stage, bool online) {
		point temp = m1.get_cord();
		int x = temp.x, y = temp.y;

		if (allStates[y][x] == edible::coin) {
			allStates[y][x] = edible::nothing;
			m1.set_money(m1.get_money() + 10);
			Mix_PlayChannel(2, music->getEffects("coin"), 0);
		}
		else if (allStates[y][x] == edible::medicine && m1.is_infected()) {
			// allStates[y][x] = edible::nothing;
			m1.set_money(m1.get_money() + 200);
			m1.set_infected(false, 0);
			Mix_PlayChannel(4, music->getEffects("med"), 0);
		}
		else if (allStates[y][x] == edible::mask) {
			allStates[y][x] = edible::nothing;
			m1.set_money(m1.get_money() + 100);
			if (stage)
			{
				m1.set_masked(true, 240);
			}
			else{
				m1.set_masked(true, 400);
			}
			Mix_PlayChannel(3, music->getEffects("mask"), 0);
		}
		else if (allStates[y][x] == edible::sanitizer) {
			allStates[y][x] = edible::nothing;
			m1.set_money(m1.get_money() + 100);
			if (stage)
			{
				m1.set_sanitized(true, 225);
			}
			else{
				m1.set_sanitized(true, 300);
			}
			Mix_PlayChannel(3, music->getEffects("mask"), 0);
		}

		if (M == mode::dual && !online) {
			point temp = m2.get_cord();
			int x = temp.x, y = temp.y;

			if (allStates[y][x] == edible::coin) {
				allStates[y][x] = edible::nothing;
				m2.set_money(m2.get_money() + 10);
				Mix_PlayChannel(2, music->getEffects("coin"), 0);
			}
			else if (allStates[y][x] == edible::medicine && m2.is_infected()) {
				// allStates[y][x] = edible::nothing;
				m2.set_money(m2.get_money() + 200);
				m2.set_infected(false, 0);
				Mix_PlayChannel(4, music->getEffects("med"), 0);
			}
			else if (allStates[y][x] == edible::mask) {
				allStates[y][x] = edible::nothing;
				m2.set_money(m2.get_money() + 100);
				if (stage)
				{
					m2.set_masked(true, 240);
				}
				else{
					m2.set_masked(true, 400);
				}
				Mix_PlayChannel(3, music->getEffects("mask"), 0);
			}
			else if (allStates[y][x] == edible::sanitizer) {
				allStates[y][x] = edible::nothing;
				m2.set_money(m2.get_money() + 100);
				if (stage)
				{
					m2.set_sanitized(true, 225);
				}
				else{
					m2.set_sanitized(true, 300);
				}
				Mix_PlayChannel(3, music->getEffects("mask"), 0);
			}
		}
		if (M == mode::dual && online){
			point temp = m2.get_cord();
			int x = temp.x, y = temp.y;

			if (allStates[y][x] == edible::coin) {
				allStates[y][x] = edible::nothing;
				// m2.set_money(m2.get_money() + 10);
				Mix_PlayChannel(2, music->getEffects("coin"), 0);
			}
			else if (allStates[y][x] == edible::medicine && m2.is_infected()) {
				// allStates[y][x] = edible::nothing;
				// m2.set_money(m2.get_money() + 200);
				// m2.set_infected(false, 0);
				Mix_PlayChannel(4, music->getEffects("med"), 0);
			}
			else if (allStates[y][x] == edible::mask) {
				allStates[y][x] = edible::nothing;
				// m2.set_money(m2.get_money() + 100);
				// if (stage)
				// {
				// 	m2.set_masked(true, 240);
				// }
				// else{
				// 	m2.set_masked(true, 400);
				// }
				Mix_PlayChannel(3, music->getEffects("mask"), 0);
			}
			else if (allStates[y][x] == edible::sanitizer) {
				allStates[y][x] = edible::nothing;
				// m2.set_money(m2.get_money() + 100);
				// if (stage)
				// {
				// 	m2.set_sanitized(true, 225);
				// }
				// else{
				// 	m2.set_sanitized(true, 300);
				// }
				Mix_PlayChannel(3, music->getEffects("mask"), 0);
			}	
		}
		
		int count = 0;
		for (int i = 0; i < block::y; i++) {
			for (int j = 0; j < block::x; j++) {
				if (allStates[i][j] == edible::coin) count++;
			}
		}

		return count == 0;
	}

	~Coin() {};

};












