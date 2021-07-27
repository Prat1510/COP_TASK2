#pragma once

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
	int life;
	direction dir;

public:
	Man(InputMaster* input_master, ImageMaster* image_master, int id): 
		Input_Master(input_master), Image_Master(image_master), ID(id) {}

	void init(){
		int size = block::size;
		if (ID == 0){
			coordinate = {2, block::y - 2};
			next_coordinate = coordinate;
			Pos_pixel_val = {coordinate.y*size, coordinate.x*size};
			dir = direction::up;
		}
		else{
			coordinate = {2, block::y - 2};
			next_coordinate = coordinate;
			Pos_pixel_val = {coordinate.y*size, coordinate.x*size};
			dir = direction::down;
		}
	}

	void show(mode M){
		int size = block::size;
		if (ID == 0){
			SDL_Texture *Texture_man1;
			switch(dir){
				case direction::up:{
					Texture_man1 = Image_Master->get_image(image_map["player1_u"]);
					break;
				}
				case direction::down:{
					Texture_man1 = Image_Master->get_image(image_map["player1_d"]);
					break;
				}
				case direction::left:{
					Texture_man1 = Image_Master->get_image(image_map["player1_l"]);
					break;
				}
				case direction::right:{
					Texture_man1 = Image_Master->get_image(image_map["player1_r"]);
					break;
				}
			}
			SDL_Rect showCoordinate = {Pos_pixel_val.x, Pos_pixel_val.y, size, size};
			SDL_Rect inputCoordinate = {0,0,size,size};
			Image_Master->render_image(*Texture_man1, inputCoordinate, showCoordinate);
			SDL_DestroyTexture(Texture_man1);
			return;
		}
		else {
			if (M == mode::dual){
				SDL_Texture *Texture_man2;
				switch(dir){
					case direction::up:{
						Texture_man2 = Image_Master->get_image(image_map["player2_u"]);
						break;
					}
					case direction::down:{
						Texture_man2 = Image_Master->get_image(image_map["player2_d"]);
						break;
					}
					case direction::left:{
						Texture_man2 = Image_Master->get_image(image_map["player2_l"]);
						break;
					}
					case direction::right:{
						Texture_man2 = Image_Master->get_image(image_map["player2_r"]);
						break;
					}
				}
				SDL_Rect showCoordinate = {Pos_pixel_val.x, Pos_pixel_val.y, size, size};
				SDL_Rect inputCoordinate = {0,0,size,size};
				Image_Master->render_image(*Texture_man2, inputCoordinate, showCoordinate);
				SDL_DestroyTexture(Texture_man2);
				return;				
			}
			return;
		}
	}

	void move(mode M, Map &map){
		if (M != mode::dual && ID != 0) return;
		int size = block::size;
		int next_pos_y = next_coordinate.y*size;
		int next_pos_x = next_coordinate.x*size;

		if (next_pos_y == Pos_pixel_val.y && next_pos_x == Pos_pixel_val.x)
		{
			coordinate = next_coordinate;
			point temp = coordinate;
			if (Input_Master->is_pressed_key(ID, 0)){
				dir = direction::up;
				temp.y++;
			}
			else if (Input_Master->is_pressed_key(ID, 1)){
				dir = direction::down;
				temp.y--;
			}
			else if (Input_Master->is_pressed_key(ID, 2)){
				dir = direction::left;
				temp.x--;
			}
			else if (Input_Master->is_pressed_key(ID, 3)){
				dir = direction::right;
				temp.x++;
			}

			if (map.FindState(temp) != Element_Map::block){
				next_coordinate = temp;
			}
		}
		else{
			if (next_pos_x > Pos_pixel_val.x){
				Pos_pixel_val.x += 2;
			}
			else if (next_pos_x < Pos_pixel_val.x){
				Pos_pixel_val.x -= 2;
			}
			if (next_pos_y > Pos_pixel_val.y){
				Pos_pixel_val.y += 2;
			}
			else if (next_pos_y < Pos_pixel_val.y){
				Pos_pixel_val.y -= 2;
			}
		}
		return;
	}

	bool is_infected(){
		return infected;
	}
	bool is_sanitized(){
		return sanitized;
	}
	bool is_masked(){
		return masked;
	}
	int get_infected(){
		return infected_val;
	}
	int get_sanitized(){
		return sanitized_val;
	}
	int get_masked(){
		return masked_val;
	}
	void set_infected(bool Infected, int x){
		infected = Infected;
		infected_val = x;
		return;
	}
	void set_sanitized(bool Sanitized, int x){
		sanitized = Sanitized;
		sanitized_val = x;
		return;
	}
	void set_masked(bool Masked, int x){
		masked = Masked;
		masked_val = x;
		return;
	}

	int get_money(){
		return money;
	}
	void set_money(int Money){
		money = Money;
		return;
	}

	int get_lives(){
		return life;
	}
	void set_lives(int Lives){
		life = Lives;
		return;
	}

	point get_pixel_val(){
		return Pos_pixel_val;
	}

	void set_pixel_val(point &val){
		Pos_pixel_val = val;
		return;
	}

	point get_cord(){
		return coordinate;
	}

	~Man(){}
};