#include "SDL.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    SDL_Window *window;                    // Declare a pointer
    SDL_Renderer *renderer;

    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "gamjam",                          // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        640,                               // width, in pixels
        480,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // The window is open: could enter program loop here (see SDL_PollEvent())
  
    bool done = false;

    while (!done) {
      SDL_Event event;

      while (SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) {
          done = true;
        }
      }

      SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
      SDL_RenderClear(renderer);
      SDL_RenderPresent(renderer);
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();

    return 0;
}
