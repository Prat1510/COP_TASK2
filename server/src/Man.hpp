#pragma once
#include "enet.h"
#include "Basic.hpp"
#include "ImageMaster.hpp"
#include "InputMaster.hpp"
#include "Map.hpp"

enum  direction {
	up = 0, down, left, right
};

class Man {
	InputMaster *Input_Master;
	ImageMaster *Image_Master;
	int ID;

	point Pos_pixel_val;
	point coordinate;
	point next_coordinate;

	int money;
	int sanitized_val;
	int masked_val;
	int infected_val;
	bool sanitized;
	bool masked;
	bool infected;
	int life = 3;
	direction dir;

public:
	Man(InputMaster* input_master, ImageMaster* image_master, int id):
		Input_Master(input_master), Image_Master(image_master), ID(id) {}

	void init() {
		int size = block::size;
		if (ID == 0) {
			coordinate = {1, block::y - 2};
			next_coordinate = coordinate;
			Pos_pixel_val = {coordinate.x * size, coordinate.y * size};
			dir = direction::up;
		}
		else {
			coordinate = {block::x - 2, 1};
			next_coordinate = coordinate;
			Pos_pixel_val = {coordinate.x * size, coordinate.y * size};
			dir = direction::down;
		}
	}

	void show(mode M) {
		int size = block::size;
		if (ID == 0) {
			SDL_Texture *Texture_man1;
			switch (dir) {
			case direction::up: {
				Texture_man1 = Image_Master->get_image(0);
				break;
			}
			case direction::down: {
				Texture_man1 = Image_Master->get_image(14);
				break;
			}
			case direction::left: {
				Texture_man1 = Image_Master->get_image(16);
				break;
			}
			case direction::right: {
				Texture_man1 = Image_Master->get_image(12);
				break;
			}
			}
			SDL_Rect showCoordinate = {Pos_pixel_val.x, Pos_pixel_val.y, size, size};
			SDL_Rect inputCoordinate = {0, 0, size, size};
			Image_Master->render_image(*Texture_man1, inputCoordinate, showCoordinate);
			SDL_DestroyTexture(Texture_man1);
			return;
		}
		else {
			if (M == mode::dual) {
				SDL_Texture *Texture_man2;
				switch (dir) {
				case direction::up: {
					Texture_man2 = Image_Master->get_image(17);
					break;
				}
				case direction::down: {
					Texture_man2 = Image_Master->get_image(1);
					break;
				}
				case direction::left: {
					Texture_man2 = Image_Master->get_image(13);
					break;
				}
				case direction::right: {
					Texture_man2 = Image_Master->get_image(15);
					break;
				}
				}
				SDL_Rect showCoordinate = {Pos_pixel_val.x, Pos_pixel_val.y, size, size};
				SDL_Rect inputCoordinate = {0, 0, size, size};
				Image_Master->render_image(*Texture_man2, inputCoordinate, showCoordinate);
				SDL_DestroyTexture(Texture_man2);
				return;
			}
			return;
		}
	}

	void move(mode M, Map &map, ENetPeer* client, bool online) {
		unsigned char msg[49];
		msg[0] = '1';
		IntToByte(Pos_pixel_val.x, &msg[1]);
		IntToByte(Pos_pixel_val.y, &msg[5]);
		if (is_infected()){
			msg[9] = '1';
		}
		IntToByte(infected_val, &msg[13]);		
		if (is_masked()){
			msg[17] = '1';
		}
		IntToByte(masked_val, &msg[21]);		
		if (is_sanitized()){
			msg[25] = '1';
		}
		IntToByte(sanitized_val, &msg[29]);	
		IntToByte(money,&msg[33]);
		IntToByte(life,&msg[37]);	
		IntToByte(coordinate.x, &msg[41]);
		IntToByte(coordinate.y, &msg[45]);
		if (online)
		{
			ENetPacket* packet = enet_packet_create(msg, 49, 0);
			enet_peer_send(client, 0 , packet);
		}
		if ((M != mode::dual || online) && ID == 1) return;
		int size = block::size;
		int next_pos_y = next_coordinate.y * size;
		int next_pos_x = next_coordinate.x * size;

		if (next_pos_y == Pos_pixel_val.y && next_pos_x == Pos_pixel_val.x)
		{
			coordinate = next_coordinate;
			point temp = coordinate;
			if (Input_Master->is_pressed_key(ID, 0)) {
				dir = direction::up;
				temp.y--;
			}
			else if (Input_Master->is_pressed_key(ID, 1)) {
				dir = direction::down;
				temp.y++;
			}
			else if (Input_Master->is_pressed_key(ID, 2)) {
				dir = direction::left;
				temp.x--;
			}
			else if (Input_Master->is_pressed_key(ID, 3)) {
				dir = direction::right;
				temp.x++;
			}

			if (map.FindState(temp) != Element_Map::block) {
				next_coordinate = temp;
			}
		}
		else {
			if (next_pos_x > Pos_pixel_val.x) {
				Pos_pixel_val.x += 2;
			}
			else if (next_pos_x < Pos_pixel_val.x) {
				Pos_pixel_val.x -= 2;
			}
			if (next_pos_y > Pos_pixel_val.y) {
				Pos_pixel_val.y += 2;
			}
			else if (next_pos_y < Pos_pixel_val.y) {
				Pos_pixel_val.y -= 2;
			}
		}
		return;
	}

	bool is_infected() {
		return infected;
	}
	bool is_sanitized() {
		return sanitized;
	}
	bool is_masked() {
		return masked;
	}
	int get_infected() {
		return infected_val;
	}
	int get_sanitized() {
		return sanitized_val;
	}
	int get_masked() {
		return masked_val;
	}
	void set_infected(bool Infected, int x) {
		infected = Infected;
		infected_val = x;
		return;
	}
	void set_sanitized(bool Sanitized, int x) {
		sanitized = Sanitized;
		sanitized_val = x;
		return;
	}
	void set_masked(bool Masked, int x) {
		masked = Masked;
		masked_val = x;
		return;
	}

	int get_money() {
		return money;
	}
	void set_money(int Money) {
		money = Money;
		return;
	}

	int get_lives() {
		return life;
	}
	void set_lives(int Lives) {
		life = Lives;
		return;
	}

	point get_pixel_val() {
		return Pos_pixel_val;
	}

	void set_pixel_val(point &val) {
		Pos_pixel_val = val;
		return;
	}

	point get_cord() {
		return coordinate;
	}
	void set_cord(int x, int y){
		coordinate.x = x;
		coordinate.y = y;
	}
	// point set
	int get_id() {
		return ID;
	}

	~Man() {}
};