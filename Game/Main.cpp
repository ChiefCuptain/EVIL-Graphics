#include "Engine.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/Pipeline.h"
#include "Resources/ResourceManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <map>
#include <memory>

using namespace nu;

namespace nu
{
    struct Color4
    {
        float r, g, b, a;
    };

}

struct Vertex
{
    Vector3 position;
    Color4 color;
};

std::vector<Vertex> vertices =
{
    Vertex{ {-0.4f, -0.6f, 0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
    Vertex{ { 0.8f, 0.2f, 0.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
    Vertex{ { 0.0f,  0.6f, 0.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } },
};

    float scale = 0.5f;
    Color4 color = { 1.0f, 0.53f, 1.0f, 1.0f };
    int main()
    {
        // don't move this
        SetWorkingDirectory("Assets");
        // don't move this


        // INITIALIZATION
        Engine::Get().Initialize();

        auto vb = std::make_shared<VertexBuffer>();
        vb->Create<Vertex>(vertices, Engine::Get().GetRenderer().GetGPUDevice());

        auto vshader = Resources().Get<nu::Shader>("shaders/position.vert", Engine::Get().GetRenderer(), 1);
        auto fshader = Resources().Get<nu::Shader>("shaders/color.frag", Engine::Get().GetRenderer(), 1);

            auto pipeline = std::make_shared<Pipeline>();
        pipeline->AddVertexBuffer(sizeof(Vertex));
        pipeline->AddVertexAttribute(
            0,
            SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
            offsetof(Vertex, position));

        pipeline->AddVertexAttribute(
            1,
            SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
            offsetof(Vertex, color));

        pipeline->Create(*vshader.get(), *fshader.get(),
            Engine::Get().GetRenderer().GetGPUDevice(),
            Engine::Get().GetRenderer().GetWindow());

        struct vertexUniform
        {
            glm::mat4 model;
        };

        vertexUniform uniform;

        float rotation = 0.0f;
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
            scale = sin(Engine::Get().GetTime().GetTime() * 30.0f);
            color.r = sin((Engine::Get().GetTime().GetTime() * 10.0f) * 0.5f + 0.5f);
            // Game

            // RENDER
            Engine::Get().GetRenderer().BeginFrame();

            Engine::Get().GetRenderer().SetPipeline(*pipeline);
            Engine::Get().GetRenderer().SetVertexBuffer(*vb);

            rotation += dt * 30.0f;
            uniform.model = glm::mat4(1.0f);
            uniform.model = glm::translate(uniform.model, glm::vec3(0.5f, 0.0f, 0.0f));
            uniform.model = glm::scale(uniform.model, glm::vec3(0.5f, 0.5f, 0.5f));
            uniform.model = glm::rotate(uniform.model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

            Engine::Get().GetRenderer().SetVertexUniform(0, &uniform, sizeof(uniform));
            Engine::Get().GetRenderer().SetFragmentUniform(0, &color, sizeof(color));
            

            Engine::Get().GetRenderer().Draw(vb->GetVertexCount());

            Engine::Get().GetRenderer().EndFrame();

        }


        // SHUTDOWN
        Engine::Get().Quit();

        // Testing edits
        return 0;
    }