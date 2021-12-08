#include <stdio.h>
#include <SDL2/SDL.h>

SDL_Window *window;
SDL_Renderer *renderer;
bool is_running;

bool initialize_window() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_BORDERLESS);

    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    return true;
}

int main(void) {
    is_running = initialize_window();
    if (!is_running) {
        fprintf(stderr, "Error initializing window.\n");    
        return -1;
    }
    fprintf(stdout, "Window initialized.\n");    
    return 0;
}