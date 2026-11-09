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

#define CHECK_UNIFORM_BOUNDARY(T) static_assert((sizeof(T) % 16) == 0);
#define MAX_LIGHTS 8

using namespace nu;

enum class LightType
{
	DIRECTIONAL,
	POINT,
	SPOT,
};

int main()
{
	// don't move this
	SetWorkingDirectory("Assets");
	// don't move this


	// INITIALIZATION
	Engine::Get().Initialize();

	Scene scene;
	scene.Load("Scenes/scene.json");

	auto model = Resources().Get<Model>("Models/tungsten_man.obj", Engine::Get().GetRenderer());
	auto texture = Resources().Get<Texture>("Textures/tungsten_base.png", Engine::Get().GetRenderer());

	auto vshader = Resources().Get<nu::Shader>("Shaders/phong.vert", Engine::Get().GetRenderer(), 4);
	auto fshader = Resources().Get<nu::Shader>("Shaders/phong.frag", Engine::Get().GetRenderer(), 4, 1);

	auto pipeline = std::make_shared<Pipeline>();

	pipeline->AddVertexBuffer(sizeof(Model::Vertex));
	pipeline->AddVertexAttribute(0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, position));
	pipeline->AddVertexAttribute(1, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offsetof(Model::Vertex, texcoord));
	pipeline->AddVertexAttribute(2, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, normal));
	pipeline->AddVertexAttribute(3, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, tangent));

	pipeline->Create(*vshader.get(), *fshader.get(),
		Engine::Get().GetRenderer().GetGPUDevice(),
		Engine::Get().GetRenderer().GetWindow());

	// UNIFORMS
	struct VertexUniform
	{
		glm::mat4 mvp;
		glm::mat4 model;
		glm::vec3 cameraPosition;
		float pad1;
	};
	CHECK_UNIFORM_BOUNDARY(VertexUniform);

	struct Light {
		glm::vec3 position;
		LightType type;
		glm::vec3 direction;
		float range;
		glm::vec3 color;
		float intensity;
		glm::vec3 ambient;
		float innerCutoff;
		float outerCutoff;
		glm::vec3 pad1;
	};

	struct LightUniform
	{
		Light lights[MAX_LIGHTS];
		uint32_t numLights;
		glm::vec3 ambient;
	};
	CHECK_UNIFORM_BOUNDARY(LightUniform);

	LightUniform lightUniform;

	Light light1{
		.position{ -5, 5, 0 },
		.type{ LightType::DIRECTIONAL },
		.direction{ 0, -1, 0 },
		.range{35.0f},
		.color{ 0.9f, 0.0f, 0.0f },
		.intensity{ 1.0f },
		.ambient{ 0.3f },
		.innerCutoff{ 0.1f },
		.outerCutoff{ 0.2f }
	};
	Light light2{
		.position{ 0, 5, 5 },
		.type{ LightType::POINT },
		.direction{ 0, -1, 0 },
		.range{35.0f},
		.color{ 0.0f, 0.9f, 0.0f },
		.intensity{ 1.0f },
		.ambient{ 0.3f },
		.innerCutoff{ 0.1f },
		.outerCutoff{ 0.2f }
	};
	Light light3{
		.position{ 5, 5, 0 },
		.type{ LightType::SPOT },
		.direction{ 0, -1, 0 },
		.range{35.0f},
		.color{ 0.0f, 0.0f, 0.9f },
		.intensity{ 1.0f },
		.ambient{ 0.3f },
		.innerCutoff{ 0.1f },
		.outerCutoff{ 0.2f }
	};

	lightUniform.lights[0] = light1;
	lightUniform.lights[1] = light2;
	lightUniform.lights[2] = light3;
	lightUniform.numLights = 3;
	lightUniform.ambient = glm::vec3(0.3f);

	struct MaterialUniform
	{
		glm::vec4 color;
		glm::vec2 tiling;
		glm::vec2 offset;
		float shininess;
		glm::vec3 pad1;
	};
	CHECK_UNIFORM_BOUNDARY(MaterialUniform);

	MaterialUniform materialUniform{
		.color{0.84f, 0.84f, 0.84f, 1.0f},
		.tiling{ 1.0f },
		.offset{ 0.0f },
		.shininess{ 128.0f }
	};




	VertexUniform vertexUniform;
	float rotation = 0.0f;
	// MAIN LOOP
	bool quit = false;

	Transform transform
	{
		.position = glm::vec3(0.0f),
		.rotation = glm::vec3(0.0f),
		.scale = glm::vec3(1),
	};

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
		float time = Engine::Get().GetTime().GetTime();

		// Scene
		scene.Update(dt);

		// RENDER
		Engine::Get().GetRenderer().BeginFrame();

		Engine::Get().GetRenderer().SetPipeline(*pipeline);

		auto camera = scene.GetActorByName<Actor>("Camera");
		auto cameraComponent = camera->GetComponent<CameraComponent>();
		vertexUniform.cameraPosition = camera->GetTransform().position;

		vertexUniform.model = transform.GetMatrix();
		vertexUniform.mvp = cameraComponent->GetProjection() * cameraComponent->GetView() * vertexUniform.model;
		Engine::Get().GetRenderer().SetVertexUniform(UniformSlot::Object, &vertexUniform, sizeof(vertexUniform));
		Engine::Get().GetRenderer().SetVertexUniform(UniformSlot::Light, &lightUniform, sizeof(lightUniform));
		Engine::Get().GetRenderer().SetVertexUniform(UniformSlot::Material, &materialUniform, sizeof(materialUniform));
		Engine::Get().GetRenderer().SetFragmentUniform(UniformSlot::Object, &vertexUniform, sizeof(vertexUniform));
		Engine::Get().GetRenderer().SetFragmentUniform(UniformSlot::Light, &lightUniform, sizeof(lightUniform));
		Engine::Get().GetRenderer().SetFragmentUniform(UniformSlot::Material, &materialUniform, sizeof(materialUniform));
		Engine::Get().GetRenderer().SetFragmentTexture(0, *texture);

		model->Draw(Engine::Get().GetRenderer());

		Engine::Get().GetRenderer().EndFrame();

	}


	// SHUTDOWN
	Engine::Get().Quit();

	// Testing edits
	return 0;
}