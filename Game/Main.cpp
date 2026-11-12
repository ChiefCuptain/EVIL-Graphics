#include "Engine.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Pipeline.h"
#include "Resources/ResourceManager.h"
#include "Framework/Scene.h"
#include "Components/CameraComponent.h"
#include "Renderer/Uniforms.h"
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

	Scene scene;
	scene.Load("Scenes/scene.json");
	auto& renderer = Engine::Get().GetRenderer();

	// MAIN LOOP
	bool quit = false;

	while (!quit) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			// UPDATE
			Engine::Get().GetEditor().ProcessEvent(event);
			if (event.type == SDL_EVENT_QUIT) {
				quit = true;
			}
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
				quit = true;
			}
		}

		// ENGINE
		Engine::Get().Update();
		float dt = Engine::Get().GetTime().GetDeltaTime();
		// SCENE
		scene.Update(dt);

		// EDITOR
		Engine::Get().GetEditor().BeginFrame();
		Engine::Get().GetEditor().Update();


		// RENDER
		renderer.BeginFrame();

		scene.Draw(renderer);
		renderer.EndRenderPass();
		Engine::Get().GetEditor().Draw(renderer);
		renderer.EndFrame();

	}


	// SHUTDOWN
	Engine::Get().Quit();

	// Testing edits
	return 0;
}