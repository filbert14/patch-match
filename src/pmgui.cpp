#include "imgui_helper.h"
#include "pm.h"

int main() {
    pm::Initialize();
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    pm::CreateWindow("pmgui", display_mode.w, display_mode.h);

    // Anne Hathaway
    pm::rgb_8::Image anne_hathaway;
    pm::rgb_8::LoadImage("anneh2.jpg", anne_hathaway);

    pm::rgb_8::ImageMatrix anne_hathaway_matrix = pm::rgb_8::GetImageMatrix(anne_hathaway);

    GLuint anne_hathaway_texture_id;
    pm::GenerateTextureRGB8(&anne_hathaway_texture_id, anne_hathaway.image_width, anne_hathaway.image_height);
    pm::UpdateTextureRGB8(&anne_hathaway_texture_id, anne_hathaway.image_data, anne_hathaway.image_width, anne_hathaway.image_height);

    // Mona Lisa
    pm::rgb_8::Image mona_lisa;
    pm::rgb_8::LoadImage("monalisa3.jpg", mona_lisa);

    pm::rgb_8::ImageMatrix mona_lisa_matrix = pm::rgb_8::GetImageMatrix(mona_lisa);

    GLuint mona_lisa_texture_id;
    pm::GenerateTextureRGB8(&mona_lisa_texture_id, mona_lisa.image_width, mona_lisa.image_height);
    pm::UpdateTextureRGB8(&mona_lisa_texture_id, mona_lisa.image_data, mona_lisa.image_width, mona_lisa.image_height);

    // Patch Match Algorithm Instance
    pm::rgb_8::PatchMatch::GetInstance().Initialize(mona_lisa_matrix, anne_hathaway_matrix, 2);
    pm::rgb_8::PatchMatch::GetInstance().ApproximateNNF(4, 0.5, 1);

    // Reconstructed Image
    pm::rgb_8::ImageMatrix reconstructed_matrix = pm::rgb_8::PatchMatch::GetInstance().Reconstruct();
    pm::rgb_8::Image reconstructed = pm::rgb_8::GetImage(reconstructed_matrix);

    GLuint reconstructed_texture_id;
    pm::GenerateTextureRGB8(&reconstructed_texture_id, reconstructed.image_width, reconstructed.image_height);
    pm::UpdateTextureRGB8(&reconstructed_texture_id, reconstructed.image_data, reconstructed.image_width, reconstructed.image_height);

    bool done = false;
    while (!done) {
        pm::HandleEvent(done);
        pm::NewFrame();

        ImGui::Begin("anne_hathaway");
        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(anne_hathaway_texture_id)), ImVec2(anne_hathaway.image_width, anne_hathaway.image_height));
        ImGui::End();

        ImGui::Begin("mona_lisa");
        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(mona_lisa_texture_id)), ImVec2(mona_lisa.image_width, mona_lisa.image_height));
        ImGui::End();

        ImGui::Begin("reconstructed");
        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(reconstructed_texture_id)), ImVec2(reconstructed.image_width, reconstructed.image_height));
        ImGui::End();

        pm::EndFrame();
    }

    pm::DestroyWindow();
    pm::Terminate();
    return 0;
}