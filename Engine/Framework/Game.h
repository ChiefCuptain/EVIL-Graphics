#pragma once
#include "Engine.h"

namespace nu
{
	class Scene;

	class Game
	{
	public:
		Game();
		~Game();

		virtual bool Initialize() { return true; }
		virtual void Quit() {}

		virtual void Update(float dt);
		virtual void Draw(class Renderer& renderer);

		void SetScene(std::unique_ptr<Scene> scene);


	protected:
		std::unique_ptr<Scene> m_scene;
	};
}