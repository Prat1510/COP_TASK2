#pragma once

#include <SDL2/SDL.h>
#include <math.h>

struct point {
	int x, y;

	point add(point &p) {
		point ans = new point();
		ans.x = p.x + x;
		ans.y = p.y + y;
		return ans;
	}

	void increment(point &p) {
		x = x + p.x;
		y = y + p.y;
	}

	bool equals(point &p) {
		return (x == p.x) && (y == p.y);
	}

	bool notEquals(point &p) {
		return (x != p.x) || (y != p.y);
	}

	int distance(point &p) {
		float x_diff = abs(x - p.x);
		float y_diff = abs(y - p.y);

		int ans = (int)sqrt( pow(x_diff, 2) + pow(y_diff, 2) );
		return ans;
	}
}


enum class mode {
	single;
	dual;
}

namespace block {

x = 24;
y = 24;
size = 20;
xsize = (count_x + 1)/2 + 1;
ysize = (count_y + 1)/2 + 1;

}

namespace display {

height = 480;
width = 640;
offset = 480;
fps = 60;
bpp = 32;

}


