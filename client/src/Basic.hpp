#pragma once

#include <SDL2/SDL.h>
#include <math.h>

struct point {
  int x, y;

  point add(point &p) {
    x = p.x + x;
    y = p.y + y;
    point ans = point{x , y};
    return ans;
  }

  point increment(point &p) {
    return point{x + p.x,y + p.y};
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
inline int ByteToInt(unsigned char* byte) {
  int n = 0;

  n = n + (byte[0] & 0x000000ff);
  n = n + ((byte[1] & 0x000000ff) << 8);
  n = n + ((byte[2] & 0x000000ff) << 16);
  n = n + ((byte[3] & 0x000000ff) << 24);


  return n;
}

inline void IntToByte(int n, unsigned char* result) {

  result[0] = n & 0x000000ff;
  result[1] = (n & 0x0000ff00) >> 8;
  result[2] = (n & 0x00ff0000) >> 16;
  result[3] = (n & 0xff000000) >> 24; 
}
