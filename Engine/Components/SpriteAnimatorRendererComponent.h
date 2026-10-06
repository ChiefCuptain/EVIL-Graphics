#pragma once
#include "SpriteRendererComponent.h"
#include "Resources/ResourceManager.h"
#include <string>
#include <map>
namespace nu
{
	class SpriteAnimatorRendererComponent : public SpriteRendererComponent
	{
	public:
		struct SpriteAnimation
		{
			std::string name;
			float fps = 10.0f;
			bool loop = true;
			res_t<class TextureFrames> textureFrames;
		};

	public:
		CLASS_PROTOTYPE(SpriteAnimatorRendererComponent)

		void Start() override;
		void Update(float dt) override;

		void Play(const std::string& name);

		bool GetPause() const { return m_pause; }
		void SetPause(bool pause = true) { m_pause = pause; }

		void Read(const json::value_t& value) override;

	private:
		unsigned int m_frame = 0;
		float m_frameTimer = 0.0f;
		bool m_pause = false;

		std::string m_defaultAnimName;

		SpriteAnimation m_currentAnim;
		std::map<std::string, SpriteAnimation> m_spriteAnims;
	};
}