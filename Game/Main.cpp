#include "Engine.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Pipeline.h"
#include "Resources/ResourceManager.h"
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

        auto model = Resources().Get<Model>("Models/spot.obj", Engine::Get().GetRenderer());
        auto texture = Resources().Get<Texture>("Textures/spot_diffuse.png", Engine::Get().GetRenderer());

        auto vshader = Resources().Get<nu::Shader>("Shaders/unlit.vert", Engine::Get().GetRenderer(), 1);
        auto fshader = Resources().Get<nu::Shader>("Shaders/unlit.frag", Engine::Get().GetRenderer(), 1, 1);

        auto pipeline = std::make_shared<Pipeline>();

        pipeline->AddVertexBuffer(sizeof(Model::Vertex));
        pipeline->AddVertexAttribute(0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, position));
        pipeline->AddVertexAttribute(1, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offsetof(Model::Vertex, texcoord));
        pipeline->AddVertexAttribute(2, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, normal));
        pipeline->AddVertexAttribute(3, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, tangent));

        pipeline->Create(*vshader.get(), *fshader.get(),
            Engine::Get().GetRenderer().GetGPUDevice(),
            Engine::Get().GetRenderer().GetWindow());

        struct vertexUniform
        {
            glm::mat4 mvp;
        };

        vertexUniform uniform;
        glm::vec3 cameraPosition{ 0.0f, 0.0f, 5.0f };
        float rotation = 0.0f;
        // MAIN LOOP
        bool quit = false;

        Transform transform;
        glm::float4 color{ 1, 1, 1, 1 };
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
            glm::vec3 direction{ 0 };

            // CAMERA MOVEMENT
            if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) direction.x = -1;
            if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) direction.x = 1;
            if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) direction.z = -1;
            if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) direction.z = 1;
            if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q)) direction.y = -1;
            if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_E)) direction.y = 1;

            cameraPosition += direction * 4.0f * dt;

            // Game

            // RENDER
            Engine::Get().GetRenderer().BeginFrame();

            Engine::Get().GetRenderer().SetPipeline(*pipeline);

            rotation += dt * 100.0f;
            //uniform.model = glm::mat4(1.0f);
            //uniform.model = glm::translate(uniform.model, glm::vec3(0.5f, 0.0f, 0.0f));
            //uniform.model = glm::rotate(uniform.model, glm::radians(rotation), glm::vec3(1.0f, 1.0f, 1.0f));
            //uniform.model = glm::scale(uniform.model, glm::vec3(0.5f, 0.5f, 0.5f));


            // TRANSFORM
            transform.rotation.y = Engine::Get().GetTime().GetTime() * 30.0f;

            // VIEW MATRIX

            auto view = glm::lookAtRH(cameraPosition, cameraPosition + glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });

            // PROJECTION MATRIX
            float aspectRatio = Engine::Get().GetRenderer().GetWindowWidth() / Engine::Get().GetRenderer().GetWindowHeight();
            auto projection = glm::perspective(glm::radians(100.0f), aspectRatio, 0.01f, 20.0f);


            uniform.mvp = projection * view * transform.GetMatrix();
            Engine::Get().GetRenderer().SetVertexUniform(0, &uniform, sizeof(uniform));
            Engine::Get().GetRenderer().SetFragmentUniform(0, &color, sizeof(color));
            Engine::Get().GetRenderer().SetFragmentTexture(0, *texture);

            model->Draw(Engine::Get().GetRenderer());

            Engine::Get().GetRenderer().EndFrame();

        }


        // SHUTDOWN
        Engine::Get().Quit();

        // Testing edits
        return 0;
    }