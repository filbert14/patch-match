#include "imgui_helper.h"

int main() {
    pm::Initialize();

    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);

    pm::CreateWindow("Test", dm.w, dm.h);

    bool done = false;

    while(!done) {
        pm::HandleEvent(done);
        pm::NewFrame();

        ImGui::ShowDemoWindow();

        pm::EndFrame();
    }

    pm::DestroyWindow();
    pm::Terminate();
}