
#include "Map.hpp"
#include <math.h>
#include <stdlib.h>
#include <string>
#include <queue>
#include "Basic.hpp"
#include "ImageMaster.hpp"
#include <iostream>
using namespace std;

Element_Map MapElementFromChar(char X) {
	if ( X == '#') { return Element_Map::block; }
	else if (X == '.') {return Element_Map::coin; }
	else if (X == 'S') {return Element_Map::sanitizer; }
	else if (X == 'P') {return Element_Map::init_p1; }
	else if (X == 'Q') {return Element_Map::init_p2; }
	else if (X == 'H') {return Element_Map::med; }
	else if (X == 'M') {return Element_Map::mask; }
	else if (X == 'E') {return Element_Map::init_v; }
	else {
		cout << "Unknown character on map." << endl;
		exit(EXIT_FAILURE);
	}
}

void Map::initialise(mode M, bool online, string MapStr) {
	string MapStringPlayer;
	if (online){
		MapStringPlayer = MapStr;
	}
	else {
		cell MAZE[xsize][ysize];
		long numin = 1;
		initialize(MAZE);   //maze generatiom
		generate(MAZE, numin);
		MapStringPlayer = savebmp(0, 0, M, MAZE, numin);
	}
	
	for (int i = 0; i < block::y ; i++) {
		for (int j = 0; j < block::x; j++) {
			ElementMap[i][j] = MapElementFromChar(MapStringPlayer[i * block::x + j]);
		}
	}
	if (M == mode::dual) {
// ElementMap[18][14] = Element_Map::init_p2;
// We have to place the player2 somewhere.
	}

	string MapStringEnemy = MapStringPlayer;
// Enemy randomly pops up when it dies-???
}

void Map::show(int stage) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_Texture *back_texture = nullptr;
	if (stage == 0) {
		// back_texture = Image_Master_Map->get_image(image_map["back1"]);
		back_texture = Image_Master_Map->get_image(9);
	}
	else if (stage == 1) {
		back_texture = Image_Master_Map->get_image(10);
	}
	else {
		back_texture = Image_Master_Map->get_image(10);
	}

	int size = block::size;
	int x_count = block::x;
	int y_count = block::y;

	SDL_Rect rect1 = {0, 0, size, size};

	for (int i = 0; i < y_count; i++) {
		for (int j = 0; j < x_count; j++) {
			if (ElementMap[i][j] == Element_Map::block){
				SDL_Rect rect2 = {j * size, i * size, size, size};
				Image_Master_Map->render_image(*back_texture, rect1, rect2);
			}
		}
	}

	SDL_DestroyTexture(back_texture);
}


