#include "imgui_helper.h"
#include "pm.h"

GLuint GenerateTexture(int image_width, int image_height, void* image_data) {
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture_id;
}

// @TODO: Write a function to delete a texture

/* *

int main() {
    pm::Initialize();
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    pm::CreateWindow("pmgui", display_mode.w, display_mode.h);

    bool done = false;
    while (!done) {
        pm::HandleEvent(done);
        pm::NewFrame();

        pm::EndFrame();
    }

    pm::DestroyWindow();
    pm::Terminate();
    return 0;
}

* */

int main() {
    pm::rgb_8::Image cat_image;
    pm::rgb_8::Image sea_image;

    pm::rgb_8::LoadImage("cat.jpg", cat_image);
    pm::rgb_8::LoadImage("sea.jpg", sea_image);

    pm::rgb_8::ImageMatrix cat_image_matrix = pm::rgb_8::GetImageMatrix(cat_image);
    pm::rgb_8::ImageMatrix sea_image_matrix = pm::rgb_8::GetImageMatrix(sea_image);

    pm::rgb_8::PatchMatch::GetInstance().Initialize(cat_image_matrix, sea_image_matrix, 1);
    pm::rgb_8::ImageMatrix reconstructed_cat_image_matrix = pm::rgb_8::PatchMatch::GetInstance().Reconstruct();
    pm::rgb_8::Image reconstructed_cat_image = pm::rgb_8::GetImage(reconstructed_cat_image_matrix);

    // GUI : START
    pm::Initialize();
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    pm::CreateWindow("pmgui", display_mode.w, display_mode.h);

    GLuint texture_id = GenerateTexture(reconstructed_cat_image.image_width, reconstructed_cat_image.image_height, reconstructed_cat_image.image_data);

    bool done = false;
    while (!done) {
        pm::HandleEvent(done);
        pm::NewFrame();

        ImGui::Begin("reconstructed_cat_image");
        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(texture_id)), ImVec2(reconstructed_cat_image.image_width, reconstructed_cat_image.image_height));
        ImGui::End();

        pm::EndFrame();
    }

    pm::DestroyWindow();
    pm::Terminate();
    // GUI : END

    pm::rgb_8::FreeImage(cat_image);
    pm::rgb_8::FreeImage(sea_image);
    return 0;
}