#include "Engine.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/Pipeline.h"
#include "Resources/ResourceManager.h"
#include <vector>
#include <map>
#include <memory>

using namespace nu;

std::vector<Vector3> vertices =
{
    Vector3{ -1.0f, -1.0f, 0.0f}, // Bottom-Left
    Vector3{  1.0f, -1.0f, 0.0f}, // Bottom-Right
    Vector3{  0.0f,  1.0f, 0.0f}, // Top-Middle
};

    int main()
    {
        // don't move this
        SetWorkingDirectory("Assets");
        // don't move this


        // INITIALIZATION
        Engine::Get().Initialize();

        auto vb = std::make_shared<VertexBuffer>();
        vb->Create<Vector3>(vertices, Engine::Get().GetRenderer().GetGPUDevice());

        auto vshader = Resources().Get<nu::Shader>("shaders/position.vert", Engine::Get().GetRenderer());
        auto fshader = Resources().Get<nu::Shader>("shaders/color.frag", Engine::Get().GetRenderer());

        auto pipeline = std::make_shared<Pipeline>();
        pipeline->AddVertexBuffer(sizeof(Vector3));
        pipeline->AddVertexAttribute(
            0,
            SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
            offsetof(Vector3, x));

        pipeline->Create(*vshader.get(), *fshader.get(),
            Engine::Get().GetRenderer().GetGPUDevice(),
            Engine::Get().GetRenderer().GetWindow());

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

            Engine::Get().GetRenderer().SetPipeline(*pipeline);
            Engine::Get().GetRenderer().SetVertexBuffer(*vb);
            Engine::Get().GetRenderer().Draw(vb->GetVertexCount());

            Engine::Get().GetRenderer().EndFrame();

        }


        // SHUTDOWN
        Engine::Get().Quit();

        // Testing edits
        return 0;
    }