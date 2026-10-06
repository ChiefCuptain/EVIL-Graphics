#include "Engine.h"
#include <vector>
#include <map>
#include <memory>

using namespace nu;

    int main()
    {
        // don't move this
        SetWorkingDirectory("Assets");
        // don't move this


        // INITIALIZATION
        Engine::Get().Initialize();

        // MAIN LOOP
        bool quit = false;

        while (!quit) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                // UPDATE
                if (event.type == SDL_EVENT_QUIT) {
                    quit = true;
                }
                if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
                    quit = true;
                }
            }

            // Engine
            Engine::Get().Update();

            float dt = Engine::Get().GetTime().GetDeltaTime();

            // Game

            // RENDER
            Engine::Get().GetRenderer().BeginFrame();
            Engine::Get().GetRenderer().Clear(); // Clear the renderer


            Engine::Get().GetPS().Draw(Engine::Get().GetRenderer());

            Engine::Get().GetRenderer().EndFrame();

        }


        // SHUTDOWN
        Engine::Get().Quit();

        // Testing edits
        return 0;
    }