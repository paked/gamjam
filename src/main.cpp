#include "SDL.h"
#include <stdio.h>

typedef int8_t    int8;
typedef int16_t   int16;
typedef int32_t   int32;
typedef int64_t   int64;
typedef uint8_t   uint8;
typedef uint16_t  uint16;
typedef uint32_t  uint32;
typedef uint64_t  uint64;
typedef float     real32;
typedef double    real64;

#define WIDTH 900
#define HEIGHT 800

enum {
  Type_None,
  Type_Sand,
  Type_Rock,
};

void update_pixel(int x, int y, uint8 map[HEIGHT][WIDTH], uint8 next[HEIGHT][WIDTH]) {
  int v = map[y][x];

  switch (v) {
    case Type_Sand:
      {
        next[y][x] = Type_None;

        next[y + 1][x] = Type_Sand;
      } break;
    case Type_Rock:
      {
        float r = (float)rand()/(float)(RAND_MAX/1);
        if (r > 0.05) {
          next[y][x] = v;
        } else {
          next[y][x] = Type_None;
        }
      } break;
  }
}

int main(int argc, char* argv[]) {
  uint8 map[HEIGHT][WIDTH] = {};
  uint8 next[HEIGHT][WIDTH] = {};

  SDL_Window *window;                    // Declare a pointer
  SDL_Renderer *renderer;
  SDL_Texture *texture;

  SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

  // Create an application window with the following settings:
  window = SDL_CreateWindow(
      "gamjam",                          // window title
      SDL_WINDOWPOS_UNDEFINED,           // initial x position
      SDL_WINDOWPOS_UNDEFINED,           // initial y position
      WIDTH,                               // width, in pixels
      HEIGHT,                               // height, in pixels
      SDL_WINDOW_OPENGL                  // flags - see below
      );


  // Check that the window was successfully created
  if (window == NULL) {
    // In the case that the window could not be made...
    printf("Could not create window: %s\n", SDL_GetError());
    return 1;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      uint8 v = Type_None;

      if (y == 0 && x % 10 == 0) {
        v = Type_Sand;
      }

      int dx = WIDTH/2 - x;
      int dy = HEIGHT/2 - y;

      if (dx*dx + dy*dy < 300) {
        v = Type_Rock;
      }

      map[y][x] = v;
    }
  }

  bool done = false;

  int i = 0;
  while (!done) {
    i ++;
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
        done = true;
      }
    }

    if (i % 2 == 0) {
      for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
          update_pixel(x, y, map, next);
        }
      }

      // TODO(harrison): do a memcpy here
      for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
          map[y][x] = next[y][x];
        }
      }
    }

    uint32* pixels;
    int pitch;

    SDL_LockTexture(texture, NULL, (void**) &pixels, &pitch);

    for (int y = 0; y < HEIGHT; y++) {
      for (int x = 0; x < WIDTH; x++) {
        uint32 color = 0x000000FF;
        uint8 v = map[y][x];
        switch (v) {
          case Type_None:
            {
              color = 0x000000FF;
            } break;
          case Type_Sand:
            {
              color = 0xFF0000FF;
            } break;
          case Type_Rock:
            {
              color = 0x00FF00FF;
            } break;
        }

        pixels[y * (pitch/4) + x] = color;
      }
    }

    SDL_UnlockTexture(texture);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  // Close and destroy the window
  SDL_DestroyWindow(window);

  // Clean up
  SDL_Quit();

  return 0;
}
