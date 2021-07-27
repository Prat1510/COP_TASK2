#pragma once

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
	vector<vector<edible>>allStates(block::y, vector<edible>(block::x));
	ImageMaster *imageMaster;
	SoundMaster *music;

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
		SDL_Texture *coin_texture = imageMaster->get_image(10);
		SDL_Texture *medicine_texture = imageMaster->get_image(12);
		SDL_Texture *mask_texture = imageMaster->get_image(14);
		SDL_Texture *sanitizer_texture = imageMaster->get_image(13);

		int sz = block::sz;
		SDL_Rect rect1 = {0, 0, sz, block::sz};

		for (int i = 0; i < block::y; i++) {
			for (int j = 0; j < block::x; j++) {

				SDL_Rect rect2 = {j * sz, i * sz, sz, sz};

				if (allStates[i][j] == edible::coin) {
					imageMaster->render_image(*coin_texture, rect1, rect2);
				}
				else if (allStates[i][j] == edible::medicine) {
					imageMaster->render_image(*medicine_texture, rect1, rect2);
				}
				else if (allStates[i][j] == edible::mask) {
					imageMaster->render_image(*mask_texture, rect1, rect2);
				}
				else if (allStates[i][j] == edible::sanitizer) {
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


	bool findState (Man &m1, Man &m2, mode M) {
		int temp = m1.get_cord();
		int x = temp.x, int y = temp.y;

		if (allStates[y][x] == edible::coin) {
			allStates[y][x] = edible::nothing;
			m1.set_money(m1.get_money() + 10);
			//play music
		}
		else if (allStates[y][x] == edible::medicine && m1.is_infected()) {
			allStates[y][x] = edible::nothing;
			m1.set_money(m1.get_money() + 50);
			m1.set_infected(false);
			//play music
		}
		else if (allStates[y][x] == edible::mask) {
			allStates[y][x] = edible::nothing;
			m1.set_money(m1.get_money() + 25);
			m1.set_mask(400);
			//play music
		}
		else if (allStates[y][x] == edible::sanitizer) {
			allStates[y][x] == edible::nothing;
			m1.set_money(m1.get_money() + 25);
			m1.set_sanitizer(400);
			//play music
		}

		if (M == 1) {
			int temp = m2.get_cord();
			int x = temp.x, int y = temp.y;

			if (allStates[y][x] == edible::coin) {
				allStates[y][x] = edible::nothing;
				m1.set_money(m1.get_money() + 10);
				//play music
			}
			else if (allStates[y][x] == edible::medicine && m1.is_infected()) {
				allStates[y][x] = edible::nothing;
				m1.set_money(m1.get_money() + 50);
				m1.set_infected(false);
				//play music
			}
			else if (allStates[y][x] == edible::mask) {
				allStates[y][x] = edible::nothing;
				m1.set_money(m1.get_money() + 25);
				m1.set_mask(400);
				//play music
			}
			else if (allStates[y][x] == edible::sanitizer) {
				allStates[y][x] == edible::nothing;
				m1.set_money(m1.get_money() + 25);
				m1.set_sanitizer(400);
				//play music
			}
		}

		//play music of sanitized
		//play music of masked

		int count = 0;
		for (int i = 0; i < block::y; i++) {
			for (int j = 0; j < block::x; j++) {
				if (allStates[i][j] == edible::coin) count++;
			}
		}

		return count == 0;
	}

	~Coin();

};












