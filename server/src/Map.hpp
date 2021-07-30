#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "Basic.hpp"
#include "ImageMaster.hpp"
using namespace std;

#define WHITE fprintf(outfile, "%c%c%c", 255,255,255)
#define BLACK fprintf(outfile, "%c%c%c", 0,0,0)
#define RED   fprintf(outfile, "%c%c%c", 0,0,255)

struct cell {
	bool in;  //Is this cell in the maze?
	bool up;  //Does the wall above this cell exist?
	bool left;//Does the wall to the left of this cell exist?
	int prevx, prevy; //The coordinates of the previous cell, used for backtracking.
};

const int xsize = block::xsize;
const int ysize = block::ysize;

inline void initialize(cell (*MAZE)[ysize]) {
	//Initialize the maze!
	for (int x = 0; x < xsize; x++) {
		for (int y = 0; y < ysize; y++) {
			//The maze cells on the edges of the maze are "in" to provide padding. Otherwise, all maze cells are not in.
			MAZE[x][y].in   = (x == 0 || x == xsize - 1 || y == 0 || y == ysize - 1) ? 1 : 0;
			//All maze cells have all walls existing by default, except the perimeter cells.
			MAZE[x][y].up   = (x == 0 || x == xsize - 1 || y == 0) ? 0 : 1;
			MAZE[x][y].left = (x == 0 || y == 0 || y == ysize - 1) ? 0 : 1;
		}
	}
	return;
}

inline void generate(cell (*MAZE)[ysize], long& numin) {
	int xcur = 1, ycur = 1; //start growing from the corner. It could theoretically start growing from anywhere, doesn't matter.
	MAZE[xcur][ycur].in = 1;
	int whichway;
	bool success;
	do {


		if ( MAZE[xcur][ycur - 1].in && MAZE[xcur][ycur + 1].in &&
		        MAZE[xcur - 1][ycur].in && MAZE[xcur + 1][ycur].in ) {
			//If at a dead end, randomly destroy a wall to make it not a dead end!
			do {
				success = 0;
				whichway = rand() % 4;
				switch (whichway) {
				case 0:
					if (MAZE[xcur][ycur].up && ycur != 1) {
						success = 1;
						MAZE[xcur][ycur].up = 0;
					}
					break;
				case 1:
					if (MAZE[xcur][ycur + 1].up && ycur != ysize - 2) {
						success = 1;
						MAZE[xcur][ycur + 1].up = 0;
					}
					break;
				case 2:
					if (MAZE[xcur][ycur].left && xcur != 1) {
						success = 1;
						MAZE[xcur][ycur].left = 0;
					}
					break;
				case 3:
					if (MAZE[xcur + 1][ycur].left && xcur != xsize - 2) {
						success = 1;
						MAZE[xcur + 1][ycur].left = 0;
					}
					break;
				}
			} while (!success);
		}


		while ( MAZE[xcur][ycur - 1].in && MAZE[xcur][ycur + 1].in &&
		        MAZE[xcur - 1][ycur].in && MAZE[xcur + 1][ycur].in ) {
			//If all the neighbourhood cells are in, backtrack.
			int xcur2 = MAZE[xcur][ycur].prevx;
			ycur = MAZE[xcur][ycur].prevy;
			xcur = xcur2;
		}
		do {
			//Randomly grow the maze if possible.
			success = 0;
			whichway = rand() % 4;
			switch (whichway) {
			case 0:
				if (!MAZE[xcur][ycur - 1].in) {
					success = 1;
					MAZE[xcur][ycur].up = 0;
					MAZE[xcur][ycur - 1].prevx = xcur;
					MAZE[xcur][ycur - 1].prevy = ycur;
					ycur--;
				}
				break;
			case 1:
				if (!MAZE[xcur][ycur + 1].in) {
					success = 1;
					MAZE[xcur][ycur + 1].up = 0;
					MAZE[xcur][ycur + 1].prevx = xcur;
					MAZE[xcur][ycur + 1].prevy = ycur;
					ycur++;
				}
				break;
			case 2:
				if (!MAZE[xcur - 1][ycur].in) {
					success = 1;
					MAZE[xcur][ycur].left = 0;
					MAZE[xcur - 1][ycur].prevx = xcur;
					MAZE[xcur - 1][ycur].prevy = ycur;
					xcur--;
				}
				break;
			case 3:
				if (!MAZE[xcur + 1][ycur].in) {
					success = 1;
					MAZE[xcur + 1][ycur].left = 0;
					MAZE[xcur + 1][ycur].prevx = xcur;
					MAZE[xcur + 1][ycur].prevy = ycur;
					xcur++;
				}
				break;
			}
		} while (!success);
		MAZE[xcur][ycur].in = 1;
		numin++; //Every iteration of this loop, one maze cell is added to the maze.
	} while (numin < (xsize - 2) * (ysize - 2));

	return;
}

inline string readBMP(char* filename) {
	FILE* f = fopen(filename, "rb");

	if (f == NULL)
		throw "Argument Exception";

	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

	// extract image height and width from header
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	cout << endl;
	cout << " Width: " << width << endl;
	cout << "Height: " << height << endl;

	int row_padded = (width * 3 + 3) & (~3);
	unsigned char* data = new unsigned char[row_padded];
	unsigned char tmp;

	string maze = "";
	for (int i = 0; i < height; i++)
	{
		fread(data, sizeof(unsigned char), row_padded, f);
		string line = "";
		for (int j = 0; j < width * 3; j += 3)
		{
			// Convert (B, G, R) to (R, G, B)
			tmp = data[j];
			data[j] = data[j + 2];
			data[j + 2] = tmp;

			if ((int)data[j] == 0) {
				line = line + "#" ;
			}
			else {
				line = line + ".";
			}
		}
		maze = line + maze;
	}

	fclose(f);
	return maze;
}

inline string savebmp(int xspecial, int yspecial, mode M, cell (*MAZE)[ysize], long& numin) {
	//save a bitmap file! the xspecial, yspecial pixel is coloured red.
	FILE * outfile;
	int extrabytes, paddedsize;
	int x, y, n;
	int width = (xsize - 1) * 2 - 1;
	int height = (ysize - 1) * 2 - 1;

	extrabytes = (4 - ((width * 3) % 4)) % 4;

	char filename[200];

	sprintf(filename, "%s_%dx%d_n%ld.bmp", "MAZE", xsize, ysize, numin);
	paddedsize = ((width * 3) + extrabytes) * height;

	unsigned int headers[13] = {static_cast<unsigned int>(paddedsize + 54), 0, 54, 40, static_cast<unsigned int>(width), static_cast<unsigned int>(height), 0, 0, static_cast<unsigned int>(paddedsize), 0, 0, 0, 0};

	outfile = fopen(filename, "wb");
	fprintf(outfile, "BM");

	for (n = 0; n <= 5; n++) {
		fprintf(outfile, "%c", headers[n] & 0x000000FF);
		fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
		fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
		fprintf(outfile, "%c", (headers[n] & (unsigned int) 0xFF000000) >> 24);
	}

	fprintf(outfile, "%c", 1); fprintf(outfile, "%c", 0);
	fprintf(outfile, "%c", 24); fprintf(outfile, "%c", 0);

	for (n = 7; n <= 12; n++) {
		fprintf(outfile, "%c", headers[n] & 0x000000FF);
		fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
		fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
		fprintf(outfile, "%c", (headers[n] & (unsigned int) 0xFF000000) >> 24);
	}

	//Actual writing of data begins here:
	for (y = 0; y <= height - 1; y++) {
		for (x = 0; x <= width - 1; x++) {
			if (x % 2 == 1 && y % 2 == 1) {
				if (x / 2 + 1 == xspecial && y / 2 + 1 == yspecial) RED;
				else {
					if (MAZE[x / 2 + 1][y / 2 + 1].in) {
						WHITE;
					} else {
						BLACK;
					}
				}
			} else if (x % 2 == 0 && y % 2 == 0) {
				BLACK;
			} else if (x % 2 == 0 && y % 2 == 1) {
				if (MAZE[x / 2 + 1][y / 2 + 1].left) {
					BLACK;
				} else {
					WHITE;
				}
			} else if (x % 2 == 1 && y % 2 == 0) {
				if (MAZE[x / 2 + 1][y / 2 + 1].up) {
					BLACK;
				} else {
					WHITE;
				}
			}
		}
		if (extrabytes) {    // See above - BMP lines must be of lengths divisible by 4.
			for (n = 1; n <= extrabytes; n++) {
				fprintf(outfile, "%c", 0);
			}
		}
	}
	// printf("file printed: %s\n", filename);
	fclose(outfile);

	int temp_x, temp_y;
	string maze = readBMP(filename);
	//placing sanitizer
	for (int i = 0; i < 4; i++) {
		temp_x = rand() % (width - 2) + 1;
		temp_y = rand() % (height - 4) + 2;
		if (maze[temp_y * width + temp_x] == '.' || maze[temp_y * width + temp_x] == '#') {
			maze[temp_y * width + temp_x] = 'S';
		}
		else {
			i--;
		}
	}

	// placing mask
	for (int i = 0; i < 4; i++) {
		temp_x = rand() % (width - 2) + 1;
		temp_y = rand() % (height - 4) + 2;
		if (maze[temp_y * width + temp_x] == '.' || maze[temp_y * width + temp_x] == '#') {
			maze[temp_y * width + temp_x] = 'M';
		}
		else {
			i--;
		}
	}

	// enemy start posn
	temp_y = height / 2;
	for (int i = 0; i < 5; i++) {
		temp_x = rand() % (width - 2) + 1;
		if (maze[temp_y * width + temp_x] == '.' || maze[temp_y * width + temp_x] == '#') {
			maze[temp_y * width + temp_x] = 'E';
		}
		else {
			i--;
		}
	}

	for (int i = 0; i < 1; i++) {
		temp_x = rand() % (width - 4) + 1;
		temp_y = rand() % (height - 4) + 1;
		if (maze[temp_y * width + temp_x] == '.' || maze[temp_y * width + temp_x] == '#') {
			maze[temp_y * width + temp_x] = 'H';
		}
		else {
			i--;
		}
	}

	maze[(height - 2)*width + 1] = 'P';

	if (M == mode::dual) {
		maze[2 * width - 2] = 'Q';
	}

	return maze;
}

enum class Element_Map {
	block,
	coin,
	sanitizer,
	init_p1,
	init_p2,
	med,
	mask,
	init_v,
};

class Map {
	SDL_Renderer* renderer;
	ImageMaster* Image_Master_Map;
	Element_Map ElementMap[block::y][block::x];

public:
	Map(SDL_Renderer* R, ImageMaster* IM): renderer(R), Image_Master_Map(IM) {}

	void initialise (mode M, ENetPeer* client, bool online);
	void show (int stage);
	Element_Map FindState(point &Point) {
		return ElementMap[Point.y][Point.x];
	}
	Element_Map FindState(point &&Point) {
		return FindState(Point);
	}
	~Map(){};
};
