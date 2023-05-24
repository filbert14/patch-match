#include "imgui_helper.h"

namespace pm {

    SDL_Window* window = nullptr;
    SDL_GLContext gl_context = nullptr;

    void Initialize() {
        // @TODO: Do a check here if an SDL subsystem failed to initialize
        SDL_Init(SDL_INIT_VIDEO);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
    }

    void Terminate() {
        ImGui::DestroyContext();
        SDL_Quit();
    }

    void CreateWindow(const char* title, int width, int height) {
        SDL_WindowFlags window_flags = (SDL_WindowFlags) (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  width, height, window_flags);

        gl_context = SDL_GL_CreateContext(window);

        SDL_GL_MakeCurrent(window, gl_context);
        SDL_GL_SetSwapInterval(1);

        ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
        ImGui_ImplOpenGL3_Init("#version 130");
    }

    void DestroyWindow() {
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
    }

    void NewFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void EndFrame() {
        ImGuiIO& io = ImGui::GetIO();
        ImVec4 clear_color = ImVec4 {0.45f, 0.55f, 0.60f, 1.0f};

        glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    void HandleEvent(bool& done) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if(event.type == SDL_QUIT) { done = true; }

            if(event.type == SDL_WINDOWEVENT &&
               event.window.event == SDL_WINDOWEVENT_CLOSE &&
               event.window.windowID == SDL_GetWindowID(window)) {
                done = true;
            }
        }
    }

    void GenerateTextureRGB8(GLuint* texture_id_ref, int image_width, int image_height) {
        glGenTextures(1, texture_id_ref);
        glBindTexture(GL_TEXTURE_2D, *texture_id_ref);

        unsigned char empty[image_width * image_height * 3];
        memset(empty, 0, image_width * image_height * 3);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8,
                     image_width, image_height, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, empty);
    }

    void UpdateTextureRGB8(GLuint* texture_id_ref, unsigned char* image_data, int image_width, int image_height) {
        glBindTexture(GL_TEXTURE_2D, *texture_id_ref);

        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                        image_width, image_height,
                        GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }

    void DeleteTexture(GLuint* texture_id_ref) {
        glDeleteTextures(1, texture_id_ref);
    }

} // pm
