#include "imgui_helper.h"
#include "stb_image_helper.h"

int main() {
    pm::Initialize();

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    pm::CreateWindow("pmgui", display_mode.w, display_mode.h);

    // Load JPG image using stb_image
    pm::Image image;
    pm::LoadImageRGB("image.jpg", image);

    int image_width = image.image_width;
    int image_height = image.image_height;
    int image_channels = image.image_channels;
    unsigned char* image_data = image.image_data;

    // Create OpenGL texture
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    bool done = false;
    while (!done) {
        pm::HandleEvent(done);
        pm::NewFrame();

        // Display the image
        ImGui::Begin("image.jpg");
        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(texture_id)), ImVec2(image_width, image_height));
        ImGui::End();

        pm::EndFrame();
    }

    // Cleanup
    pm::FreeImage(image);

    pm::DestroyWindow();
    pm::Terminate();

    return 0;
}