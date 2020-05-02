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

enum Type : uint8 {
  Type_None,
  Type_Sand,
  Type_Rock,
  Type_Bedrock,
};

Type get(Type map[HEIGHT][WIDTH], int x, int y) {
  if (x < 0 || y < 0 || x == HEIGHT || y == HEIGHT) {
    return Type_Bedrock;
  }

  return map[y][x];
}

void set(Type map[HEIGHT][WIDTH], int x, int y, Type v) {
  if (x < 0 || y < 0 || x == HEIGHT || y == HEIGHT) {
    return;
  }

  map[y][x] = v;
}

bool solid(Type t) {
  return t == Type_Bedrock || t == Type_Rock | t == Type_Sand;
}

void update_pixel(int x, int y, Type map[HEIGHT][WIDTH], Type next[HEIGHT][WIDTH]) {
  Type v = map[y][x];

  switch (v) {
    case Type_Sand:
      {
        if (solid(get(map, x, y + 1))) {
          if (!solid(get(map, x - 1, y + 1))) {
            set(next, x, y, Type_None);
            x -= 1;
            y += 1;
          } else if (!solid(get(map, x + 1, y + 1))) {
            set(next, x, y, Type_None);
            x += 1;
            y += 1;
          }

          set(next, x, y, Type_Sand);
        } else {
          set(next, x, y, Type_None);
          set(next, x, y + 1, Type_Sand);
        }
      } break;
    case Type_Rock:
      {
          set(next, x, y, v);
      } break;
  }
}

int main(int argc, char* argv[]) {
  Type map[HEIGHT][WIDTH] = {};
  Type next[HEIGHT][WIDTH] = {};

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
      Type v = Type_None;

      if (y == HEIGHT- 1) {
        v = Type_Rock;
      }

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
  

  int mouseX, mouseY;
  uint32 buttonState;

  while (!done) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
        done = true;
      }

      buttonState = SDL_GetMouseState(&mouseX, &mouseY); 
      printf("%i, %i, %i\n", mouseX, mouseY, buttonState);  
      
    }

    /*
    for (int x = 0; x < WIDTH; x++) {
      if (x % 10 == 0) {
        set(map, x, 0, Type_Sand);
      }
    }*/
    set(map, WIDTH/2, HEIGHT-100, Type_Sand);

    for (int y = 0; y < HEIGHT; y++) {
      for (int x = 0; x < WIDTH; x++) {
        update_pixel(x, y, map, next);

        if ((mouseX-x)*(mouseX-x) + (mouseY-y)*(mouseY-y) < 81) {
          if (buttonState == 1) {
            set(map, x, y, Type_Sand);
          } 
        }
      }
    }

    memcpy(map, next, sizeof(next));

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
          default:
            {} break;
        }
         
        if ((mouseX-x)*(mouseX-x) + (mouseY-y)*(mouseY-y) < 81) {
          if ((mouseX-x)*(mouseX-x) + (mouseY-y)*(mouseY-y) > 64) {
            color = 0xFFFFFFFF;
          }
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
