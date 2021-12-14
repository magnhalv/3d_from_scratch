#include "display.h"

u32 window_height = 600;
u32 window_width = 800;
SDL_Window *window;
SDL_Renderer *renderer;
u32 *color_buffer;
SDL_Texture *color_buffer_texture;

bool initialize_window()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);

    window_width = 801;  // display_mode.w;
    window_height = 601; // display_mode.h;

    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_BORDERLESS);

    if (!window)
    {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer)
    {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

void render_color_buffer()
{
    SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, sizeof(u32) * window_width);
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

inline void draw_pixel(u32 x, u32 y, u32 color)
{
    if((x >= 0 && x < window_width) && (y >= 0 && y < window_height)) {
        color_buffer[y * window_width + x] = color;
    }
    
}

void clear_color_buffer(u32 color)
{
    for (u32 y = 0; y < window_height; y++)
    {
        for (u32 x = 0; x < window_width; x++)
        {
            draw_pixel(x, y, color);
        }
    }
}

void draw_rect(u32 pos_x, u32 pos_y, u32 width, u32 height, u32 color)
{
    for (u32 y = 0; y < height; y++)
    {        
        for (u32 x = 0; x < width; x++)
        {
            draw_pixel(pos_x + x, pos_y + y, color);
        }        
    }
}

void draw_grid(void)
{
    clear_color_buffer(0xFF000000);
    u32 grid_color = 0xFF222222;
    u32 interval = 20;
    for (u32 y = 0; y < window_height; y++)
    {
        for (u32 x = 0; x < window_width; x++)
        {
            if (y % interval == 0 || x % interval == 0)
            {
                draw_pixel(x, y, grid_color);
            }
        }
    }
}

void destroy_window(void)
{
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}