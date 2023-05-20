#ifndef PM_PM_H
#define PM_PM_H

#include "SDL2/SDL.h"
#include <SDL_opengl.h>

namespace pm {

    SDL_Window* window = nullptr;
    SDL_GLContext gl_context = nullptr;

    struct Image {
        int image_width;
        int image_height;
        int image_channels;
        unsigned char* image_data;
    };

} // pm

#endif //PM_PM_H