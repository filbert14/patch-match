#ifndef PM_IMGUI_HELPER_H
#define PM_IMGUI_HELPER_H

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "SDL2/SDL.h"
#include <SDL_opengl.h>

namespace pm {

    void Initialize();
    void Terminate();
    void CreateWindow(const char* title, int width, int height);
    void DestroyWindow();
    void NewFrame();
    void EndFrame();
    void HandleEvent(bool& done);
    void GenerateTextureRGB8(GLuint* texture_id_ref, int image_width, int image_height);
    void UpdateTextureRGB8(GLuint* texture_id_ref, unsigned char* image_data, int image_width, int image_height);
    void DeleteTexture(GLuint* texture_id_ref);

} // pm

#endif //PM_IMGUI_HELPER_H