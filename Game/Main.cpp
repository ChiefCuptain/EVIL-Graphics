#include "DinkiverseSandbox/DinkiverseSandbox.h"
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
        std::unique_ptr<Game> game = std::make_unique<DinkiverseSandbox>();
        game->Initialize();

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
            game->Update(dt);

            // RENDER
            Engine::Get().GetRenderer().SetColor(0, 10, 20);
            Engine::Get().GetRenderer().Clear(); // Clear the renderer

            game->Draw(Engine::Get().GetRenderer());

            Engine::Get().GetPS().Draw(Engine::Get().GetRenderer());

            Engine::Get().GetRenderer().RenderPresent();// Render the screen
        }

        game.reset();

        // SHUTDOWN
        Engine::Get().Quit();

        // Testing edits
        return 0;
    }