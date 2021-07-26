#pragma once

#include <SDL2/SDL.h>
#include <math.h>

struct point {
  int x, y;

  point add(point &p) {
    int x = p.x + x;
    int y = p.y + y;
    point ans = point{x , y};
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
};


enum class mode {
  single = 0,
  dual,
};

namespace block {

enum {
  x = 23,
  y = 23,
  size = 20,
  xsize = (x + 1) / 2 + 1,
  ysize = (y + 1) / 2 + 1,
};

}

namespace display {

enum {
  height = 23 * 20 + 120,
  width = 23 * 20,
  bar = 23 * 20,
  fps = 60,
  bpp = 32,
};

}