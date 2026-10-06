#pragma once
#include "Resources/Resource.h"

#include <string>

struct TTF_Font;

namespace nu
{
	class Font : public Resource
	{
	public:
		Font() = default;
		~Font();

		bool Load(const std::string& name, float fontSize);

		bool ChangeFontSize(const std::string& name, float newFontSize);

	private:
		friend class Text;

		TTF_Font* m_ttfFont{ nullptr };
	};
}