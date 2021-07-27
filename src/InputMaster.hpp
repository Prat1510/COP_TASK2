#pragma once

#include <iostream>
#include <vector>
#include "Basic.hpp"
using namespace std;

// 	left = 0, right = 1, up = 2, down = 3
// 	h = 4, start = 5, select = 6, space = 7


class InputMaster {
	int m = 2, n = 8;
	vector<vector<bool>> edge_key = { {false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false}
	};

	vector<vector<bool>> pressed_key = { {false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false}
	};


public :

	InputMaster() {}

	bool is_edge_key(int player_type, int button) {
		return edge_key[player_type][button];
	}

	bool id_pressed_key(int player_type, int button) {
		return pressed_key[player_type][button];
	}

	void update() {
		vector<vector<bool>> new_key(2, vector<bool>(8, false));

		const Uint8 *keyboard = SDL_GetKeyboardState(nullptr);

		new_key[0][0] = (SDL_PRESSED == keyboard[SDL_SCANCODE_LEFT]);
		new_key[0][1] = (SDL_PRESSED == keyboard[SDL_SCANCODE_RIGHT]);
		new_key[0][2] = (SDL_PRESSED == keyboard[SDL_SCANCODE_UP]);
		new_key[0][3] = (SDL_PRESSED == keyboard[SDL_SCANCODE_DOWN]);
		new_key[0][6] = (SDL_PRESSED == keyboard[SDL_SCANCODE_RETURN]);
		new_key[0][7] = (SDL_PRESSED == keyboard[SDL_SCANCODE_SPACE]);

		new_key[1][0] = (SDL_PRESSED == keyboard[SDL_SCANCODE_W]);
		new_key[1][1] = (SDL_PRESSED == keyboard[SDL_SCANCODE_A]);
		new_key[1][2] = (SDL_PRESSED == keyboard[SDL_SCANCODE_S]);
		new_key[1][3] = (SDL_PRESSED == keyboard[SDL_SCANCODE_D]);
		new_key[1][6] = (SDL_PRESSED == keyboard[SDL_SCANCODE_RETURN]);
		new_key[1][7] = (SDL_PRESSED == keyboard[SDL_SCANCODE_SPACE]);

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j <= 7; j++) {
				edge_key[i][j] = new_key[i][j] && (!pressed_key[i][j]);
				pressed_key[i][j] = new_key[i][j];
			}
		}
	}

	~InputMaster() {}

};




