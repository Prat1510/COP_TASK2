#pragma once

#include <SDL2/SDL_mixer.h>
#include <memory>
#include <vector>
#include "Basic.hpp"
#include "ImageMaster.hpp"
#include "Map.hpp"
#include "SoundMaster.hpp"
#include "Man.hpp"
using namespace std;

struct monoVirus{
	point pixel_Cord;
	point Cord;
	point next_Cord;
	direction dir;
	int state;	
};

class Virus
{
	vector<monoVirus> viruses;
	ImageMaster* Image_Master;
	SoundMaster* Sound_Master;
	int count = 5;

	void refresh(){
		for (int i = 0; i < count; i++){
			if (viruses[i].state == 0){
				viruses.erase(viruses.begin() + i);
				count--;
				i--;
			}
		}
	}

	vector<point> dir_array(int dir){
		if (dir == 0){
			return {point{0,-1},point{0,1},point{-1,0},point{1,0}};
		}
		if (dir == 1){
			return {point{0,1},point{0,-1},point{1,0},point{-1,0}};
		}
		if (dir == 2){
			return {point{-1,0},point{1,0},point{0,-1},point{0,1}};
		}
		if (dir == 3){
			return {point{1,0},point{-1,0},point{0,1},point{0,-1}};
		}
	}
	void move_random_virus(monoVirus &virus,Map &map, Man &player1, Man &player2){
		int size = block::size;
		point pixel_next = {virus.next_Cord.x*size, virus.next_Cord.y*size};
		if (virus.pixel_Cord.notEquals(pixel_next)){
			if (virus.pixel_Cord.x > pixel_next.x){
				virus.pixel_Cord.x -= 2;
			}
			else if (virus.pixel_Cord.x < pixel_next.x)
			{
				virus.pixel_Cord.x += 2;
			}
			if (virus.pixel_Cord.y > pixel_next.y){
				virus.pixel_Cord.y -= 2;
			}
			else if (virus.pixel_Cord.y < pixel_next.y)
			{
				virus.pixel_Cord.y += 2;
			}
		}
		else{
			vector<point> D_arr = dir_array((int) virus.dir);
			point frontCord = virus.Cord.increment(D_arr[0]);
			point backCord = virus.Cord.increment(D_arr[1]);
			point leftCord = virus.Cord.increment(D_arr[2]);
			point rightCord = virus.Cord.increment(D_arr[3]);
			
			bool isFront = map.FindState(frontCord) != Element_Map::block;
			bool isBack = map.FindState(backCord) != Element_Map::block;
			bool isLeft = map.FindState(leftCord) != Element_Map::block;
			bool isRight = map.FindState(rightCord) != Element_Map::block;

			if (isFront){
				if (isLeft && isRight){
					switch(rand()%3){
						case 0:
							virus.next_Cord = frontCord;
							break;
						case 1:
							virus.next_Cord = leftCord;
							break;
						default:
							virus.next_Cord = rightCord;
					}
				}
				else if (isRight){
					switch(rand()%2){
						case 0:
							virus.next_Cord = frontCord;
							break;
						default:
							virus.next_Cord = rightCord;
					}
				}
				else if (isLeft){
					switch(rand()%2){
						case 0:
							virus.next_Cord = frontCord;
							break;
						default:
							virus.next_Cord = leftCord;
					}
				}
				else{
					virus.next_Cord = frontCord;
				}
			}
			else{
				if (isLeft && isRight){
					switch(rand()%2){
						case 0:
							virus.next_Cord = leftCord;
							break;
						default:
							virus.next_Cord = rightCord;
					}
				}
				if (isRight){
					virus.next_Cord = rightCord;
				}
				if (isLeft){
					virus.next_Cord = leftCord;
				}
				virus.next_Cord = backCord;
			}
		return;
		}
	}

	void move_seek_virus(monoVirus &virus,Map &map, Man &player1, Man &player2){

	}

public:
	Virus(ImageMaster* image_master, SoundMaster* sound_master):Image_Master(image_master), Sound_Master(sound_master){;
		for (int i = 0; i < count; ++i)
		{
			monoVirus Monovirus_temp = {};
			viruses.push_back(Monovirus_temp);
		}
	}

	void init(Map &map){
		vector<point> start;
		for (int i = 0; i < block::y; ++i)
		{
			for (int j = 0; j < block::x; ++j)
			{
				if (map.FindState(point{i,j}) == Element_Map::init_v){
					start.push_back({i,j});
				}
			}
		}
		for (int i = 0; i < count; ++i)
		{
			viruses[i].pixel_Cord = {block::size*start[i].x, block::size*start[i].y};
			viruses[i].Cord = start[i];
			viruses[i].next_Cord = viruses[i].Cord;
			switch (i%4){
				case(0): viruses[i].dir = direction::up;
						break;
				case(1): viruses[i].dir = direction::down;
						break;
				case(2): viruses[i].dir = direction::left;
						break;
				case(3): viruses[i].dir = direction::right;
						break;

			}
			// viruses[i].dir = i%4;
			viruses[i].state = 1;
		}
	}

	void show(){
		SDL_Texture* virus_texture;
		virus_texture = Image_Master->get_image(image_map["virus_icon"]);
		SDL_Rect inputCord = {0,0,block::size,block::size};
		for (int i = 0; i < count; ++i)
		{
			SDL_Rect showCord = {block::size*viruses[i].pixel_Cord.x,block::size*viruses[i].pixel_Cord.y,block::size,block::size};
			Image_Master->render_image(*virus_texture, inputCord, showCord);
		}
		SDL_DestroyTexture(virus_texture);
	}

	void move(int stage, Map &map, Man &player1, Man &player2){
		refresh();	
		for (int i = 0; i < count; ++i)
		{
			if (!stage)
			{
				move_random_virus(viruses[i], map, player1, player2);
			}
			else if (stage == 1){
				move_seek_virus(viruses[i], map, player1, player2);
			}
		}
	}

	bool virus_catch_man(mode M, Man &player1, Man &player2){
		point p1_pix_val = player1.get_pixel_val();
		monoVirus* temp;
		int dist;
		for (int i = 0; i < count; i++){
			temp = &viruses[i];
			dist = p1_pix_val.distance(temp->pixel_Cord);
			if (dist < block::size){
				if (player1.is_sanitized()){
					temp->state = 0;
					player1.set_money(player1.get_money()+1000);
					// Play virus death music
					// Mix_PlayChannel(se_type::beat_enemy, mixer_manager_->get_se(se_type::beat_enemy), 0);
				}
				else{
					if (!player1.is_masked())
					{
						if (!player1.is_infected())
						{
							player1.set_infected(true, 300);
							return true;
						}
						else
						{
							player1.set_infected(true, player1.get_infected()/2);
							return true;
						}	
					}
				}
			}
		}

		if (M != mode::dual)
		{
			return false;
		}
		point p2_pix_val = player2.get_pixel_val();
		for (int i = 0; i < count; i++){
			temp = &viruses[i];
			dist = p2_pix_val.distance(temp->pixel_Cord);
			if (dist < block::size){
				if (player2.is_sanitized()){
					temp->state = 0;
					player2.set_money(player1.get_money()+1000);
					// Play virus death music
					// Mix_PlayChannel(se_type::beat_enemy, mixer_manager_->get_se(se_type::beat_enemy), 0);
				}
				else{
					if (!player2.is_masked())
					{
						if (!player2.is_infected())
						{
							player1.set_infected(true, 300);
							return true;
						}
						else
						{
							player1.set_infected(true, player2.get_infected()/2);
							return true;
						}	
					}
				}
			}
		}
		return false;
	}
	~Virus();
	
};