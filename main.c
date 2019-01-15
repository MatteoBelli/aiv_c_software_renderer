#include "aiv_renderer.h"
#include <stdlib.h>
#define SDL_MAIN_HANDLED
#include "sdl/SDL.h"

#define triangle(x0, y0, z0, x1, y1, z1, x2, y2, z2) \
    triangle_new(                                    \
        vertex_new(vector3_new(x0, y0, z0)),         \
        vertex_new(vector3_new(x1, y1, z1)),         \
        vertex_new(vector3_new(x2, y2, z2)))

int main(int argc, char const *argv[])
{
    context_t context = context_new(600, 600);

    triangle_t triangle = triangle(0, 0.5, 0, 0, -0.5, 0, -0.5, 0, 0);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, 0);
    if (!window)
        return -1;

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
        return -1;

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 600, 600);
    if (!texture)
        return -1;

    for (;;)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                return 0;

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_DOWN)
                    context.camera.y += 0.1;
                else if (event.key.keysym.sym == SDLK_UP)
                    context.camera.y -= 0.1;
                else if (event.key.keysym.sym == SDLK_RIGHT)
                    context.camera.x -= 0.1;
                else if (event.key.keysym.sym == SDLK_LEFT)
                    context.camera.x += 0.1;
            }
        }

        int pitch;
        SDL_LockTexture(texture, NULL, (void **)&context.framebuffer, &pitch);
        rasterize(&context, &triangle);
        SDL_UnlockTexture(texture);

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        clear_buffer(&context, pitch * context.height);
    }

    return 0;
}