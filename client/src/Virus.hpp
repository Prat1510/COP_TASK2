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

struct monoVirus {
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

	void refresh() {
		for (int i = 0; i < count; i++) {
			if (viruses[i].state == 0) {
				viruses.erase(viruses.begin() + i);
				count--;
				i--;
			}
		}
	}

	vector<point> dir_array(int dir) {
		if (dir == 0) {
			return {point{0, -1}, point{0, 1}, point{ -1, 0}, point{1, 0}};
		}
		if (dir == 1) {
			return {point{0, 1}, point{0, -1}, point{1, 0}, point{ -1, 0}};
		}
		if (dir == 2) {
			return {point{ -1, 0}, point{1, 0}, point{0, 1}, point{0, -1}};
		}
		else {
			return {point{1, 0}, point{ -1, 0}, point{0, -1}, point{0, 1}};
		}
	}

	void move_random_virus(monoVirus &virus, Map &map) {
		int size = block::size;
		point pixel_next = {virus.next_Cord.x * size, virus.next_Cord.y * size};
		if (virus.pixel_Cord.notEquals(pixel_next)) {
			if (virus.pixel_Cord.x > pixel_next.x) {
				virus.pixel_Cord.x -= 2;
			}
			else if (virus.pixel_Cord.x < pixel_next.x)
			{
				virus.pixel_Cord.x += 2;
			}
			if (virus.pixel_Cord.y > pixel_next.y) {
				virus.pixel_Cord.y -= 2;
			}
			else if (virus.pixel_Cord.y < pixel_next.y)
			{
				virus.pixel_Cord.y += 2;
			}
		}
		else {
			virus.Cord = virus.next_Cord;
			vector<point> D_arr = dir_array((int) virus.dir);
			point frontCord = {virus.Cord.x + D_arr[0].x, virus.Cord.y + D_arr[0].y};
			point backCord = {virus.Cord.x + D_arr[1].x, virus.Cord.y + D_arr[1].y};
			point leftCord = {virus.Cord.x + D_arr[2].x, virus.Cord.y + D_arr[2].y};
			point rightCord = {virus.Cord.x + D_arr[3].x, virus.Cord.y + D_arr[3].y};

			bool isFront = map.FindState(frontCord) != Element_Map::block;
			bool isLeft = map.FindState(leftCord) != Element_Map::block;
			bool isRight = map.FindState(rightCord) != Element_Map::block;

			if (isFront) {
				if (isLeft && isRight) {
					switch (rand() % 3) {
					case 0:
						virus.next_Cord = frontCord;
						break;
					case 1:
						virus.next_Cord = leftCord;
						break;
					default:
						virus.next_Cord = rightCord;
						break;
					}
				}
				else if (isRight) {
					switch (rand() % 2) {
					case 0:
						virus.next_Cord = frontCord;
						break;
					default:
						virus.next_Cord = rightCord;
						break;
					}
				}
				else if (isLeft) {
					switch (rand() % 2) {
					case 0:
						virus.next_Cord = frontCord;
						break;
					default:
						virus.next_Cord = leftCord;
						break;
					}
				}
				else {
					virus.next_Cord = frontCord;
				}
			}
			else {
				if (isLeft && isRight) {
					switch (rand() % 2) {
					case 0:
						virus.next_Cord = leftCord;
						break;
					default:
						virus.next_Cord = rightCord;
						break;
					}
				}
				else if (isRight) {
					virus.next_Cord = rightCord;
				}
				else if (isLeft) {
					virus.next_Cord = leftCord;
				}
				else {
					virus.next_Cord = backCord;
				}
			}
			point diff = {virus.next_Cord.x - virus.Cord.x, virus.next_Cord.y - virus.Cord.y};
			if (diff.x > 0) {
				virus.dir = direction::right;
			}
			else if (diff.x < 0) {
				virus.dir = direction::left;
			}
			if (diff.y < 0) {
				virus.dir = direction::up;
			}
			else if (diff.y > 0) {
				virus.dir = direction::down;
			}
			return;
		}
	}

	// void move_seek_virus(monoVirus &virus, Map &map) {

	// }

public:
	Virus(ImageMaster* image_master, SoundMaster* sound_master): Image_Master(image_master), Sound_Master(sound_master) {
		;
		for (int i = 0; i < count; ++i)
		{
			monoVirus Monovirus_temp = {};
			viruses.push_back(Monovirus_temp);
		}
	}

	void init(Map &map) {
		vector<point> start;
		for (int i = 0; i < block::y; ++i)
		{
			for (int j = 0; j < block::x; ++j)
			{
				if (map.FindState(point{i, j}) == Element_Map::init_v) {
					start.push_back({i, j});
				}
			}
		}

		while (count < 5) {
			monoVirus Monovirus_temp = {};
			viruses.push_back(Monovirus_temp);
			count++;
		}

		for (int i = 0; i < count; ++i)
		{
			viruses[i].pixel_Cord = {block::size*start[i].x, block::size*start[i].y};
			viruses[i].Cord = start[i];
			viruses[i].next_Cord = viruses[i].Cord;
			switch (i % 4) {
			case (0): viruses[i].dir = direction::up;
				break;
			case (1): viruses[i].dir = direction::down;
				break;
			case (2): viruses[i].dir = direction::left;
				break;
			case (3): viruses[i].dir = direction::right;
				break;

			}
			// viruses[i].dir = i%4;
			viruses[i].state = 1;
		}
	}

	void show() {
		refresh();
		SDL_Texture* virus_texture;
		virus_texture = Image_Master->get_image(5);
		SDL_Rect inputCord = {0, 0, block::size, block::size};
		for (int i = 0; i < count; ++i)
		{
			SDL_Rect showCord = {viruses[i].pixel_Cord.x, viruses[i].pixel_Cord.y, block::size, block::size};
			Image_Master->render_image(*virus_texture, inputCord, showCord);
		}
		SDL_DestroyTexture(virus_texture);
	}

	void move(int stage, Map &map, bool online, ENetPeer* client) {
		refresh();
		unsigned char msg[viruses.size()*8 + 1];
		msg[0] = '3';
		for (int i = 0; i < viruses.size(); i++)
		{
			IntToByte(viruses[i].pixel_Cord.x, &msg[i*8 + 1]);
			IntToByte(viruses[i].pixel_Cord.y, &msg[i*8 + 5]);	
		}
		if (online)
		{
			ENetPacket* packet = enet_packet_create(msg, 8*viruses.size()+1, 0);
			enet_peer_send(client, 0 , packet);
		}

		for (int i = 0; i < count; ++i)
		{
			if (!stage)
			{
				move_random_virus(viruses[i], map);
			}
			else if (stage == 1) {
				move_random_virus(viruses[i], map);
			}
		}
	}

	bool virus_catch_man(mode M, Man &player, ENetPeer* server) {

		if (M != mode::dual && player.get_id() == 1) return false;

		point p1_pix_val = player.get_pixel_val();
		monoVirus* temp;
		int dist;

		for (int i = 0; i < count; i++) {
			temp = &viruses[i];
			dist = p1_pix_val.distance(temp->pixel_Cord);

			if (dist < block::size) {
				if (player.is_sanitized()) {
					// temp->state = 0;
					player.set_money(player.get_money() + 500);
					unsigned char msg[5];
					msg[0] = '4';
					IntToByte(i, &msg[1]);
					ENetPacket* packet = enet_packet_create(msg, 5, 0);
					enet_peer_send(server, 0 , packet);
					refresh();
					Mix_PlayChannel(6, Sound_Master->getEffects("virus"), 0);
				}
				else {
					if (!player.is_masked())
					{
						Mix_PlayChannel(6, Sound_Master->getEffects("virus"), 0);

						if (!player.is_infected())
						{
							player.set_infected(true, 500);
							return true;
						}
						else
						{
							player.set_infected(true, player.get_infected() - 1);
							return true;
						}
					}
				}
			}
		}

		return false;
	}

	void setState(int ID, int state){
		viruses[ID].state = state;
	}
	void setPixel(int ID, int x, int y){
		viruses[ID].pixel_Cord.x = x;
		viruses[ID].pixel_Cord.y = y;	
	}
	int getSize(){
		return viruses.size();
	}
	~Virus() {};

};