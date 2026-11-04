#include "Engine.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Pipeline.h"
#include "Resources/ResourceManager.h"
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

	auto model = Resources().Get<Model>("Models/tungsten_man.obj", Engine::Get().GetRenderer());
	auto texture = Resources().Get<Texture>("Textures/tungsten_base.png", Engine::Get().GetRenderer());

	auto vshader = Resources().Get<nu::Shader>("Shaders/phong.vert", Engine::Get().GetRenderer(), 3);
	auto fshader = Resources().Get<nu::Shader>("Shaders/phong.frag", Engine::Get().GetRenderer(), 3, 1);

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
	vertexUniform.cameraPosition = glm::vec3{ 1.5f, 1.5f, 2.0f };
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
		glm::vec3 direction{ 0 };

		// CAMERA MOVEMENT
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) direction.x = -1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) direction.x = 1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) direction.z = -1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) direction.z = 1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q)) direction.y = -1;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_E)) direction.y = 1;

	/*	if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_1))
		{
			lightUniform.type = LightType::DIRECTIONAL;
			lightUniform.position = glm::vec3(0.0f, 5.0f, 0.0f);
			lightUniform.direction = { 0, -1, 0 };
		}
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_2))
		{
			lightUniform.type = LightType::POINT;
			lightUniform.position = glm::vec3(0.0f, 5.0f, 0.0f);
			lightUniform.direction = { 0, -1, 0 };
		}
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_3))
		{
			lightUniform.type = LightType::SPOT;
			lightUniform.position = glm::vec3(0.0f, 5.0f, 0.0f);
			lightUniform.direction = { -1, -1, 0 };
			lightUniform.innerCutoff = cos(glm::radians(15.0f));
			lightUniform.outerCutoff = cos(glm::radians(25.0f));
		}*/


		vertexUniform.cameraPosition += direction * 4.0f * dt;

		//lightUniform.lights[0].position = glm::eulerAngleY(time) * glm::vec4(-5, 5, 0, 1);
		//lightUniform.lights[1].position = glm::eulerAngleX(time * 0.5f) * glm::vec4(0, 5, 5, 1);
		//lightUniform.lights[2].position = glm::eulerAngleZ(time * 1.2f) * glm::vec4(5, 5, 0, 1);

		// Game

		// RENDER
		Engine::Get().GetRenderer().BeginFrame();

		Engine::Get().GetRenderer().SetPipeline(*pipeline);

		rotation += dt * 100.0f;
		//uniform.model = glm::mat4(1.0f);
		//uniform.model = glm::translate(uniform.model, glm::vec3(0.5f, 0.0f, 0.0f));
		//uniform.model = glm::rotate(uniform.model, glm::radians(rotation), glm::vec3(1.0f, 1.0f, 1.0f));
		//uniform.model = glm::scale(uniform.model, glm::vec3(0.5f, 0.5f, 0.5f));


		// VIEW MATRIX

		auto view = glm::lookAtRH(vertexUniform.cameraPosition, vertexUniform.cameraPosition + glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });

		// PROJECTION MATRIX
		float aspectRatio = Engine::Get().GetRenderer().GetWindowWidth() / Engine::Get().GetRenderer().GetWindowHeight();
		auto projection = glm::perspective(glm::radians(100.0f), aspectRatio, 0.01f, 200.0f);

		vertexUniform.model = transform.GetMatrix();
		vertexUniform.mvp = projection * view * vertexUniform.model;
		Engine::Get().GetRenderer().SetVertexUniform(0, &vertexUniform, sizeof(vertexUniform));
		Engine::Get().GetRenderer().SetVertexUniform(1, &lightUniform, sizeof(lightUniform));
		Engine::Get().GetRenderer().SetVertexUniform(2, &materialUniform, sizeof(materialUniform));
		Engine::Get().GetRenderer().SetFragmentUniform(0, &vertexUniform, sizeof(vertexUniform));
		Engine::Get().GetRenderer().SetFragmentUniform(1, &lightUniform, sizeof(lightUniform));
		Engine::Get().GetRenderer().SetFragmentUniform(2, &materialUniform, sizeof(materialUniform));
		Engine::Get().GetRenderer().SetFragmentTexture(0, *texture);

		model->Draw(Engine::Get().GetRenderer());

		Engine::Get().GetRenderer().EndFrame();

	}


	// SHUTDOWN
	Engine::Get().Quit();

	// Testing edits
	return 0;
}