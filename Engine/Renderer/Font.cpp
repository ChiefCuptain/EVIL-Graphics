#include "pch.h"
#include "Font.h"


namespace nu
{
	Font::~Font() {
		if (m_ttfFont != nullptr) {
			TTF_CloseFont(m_ttfFont);
		}
	}

	bool Font::Load(const std::string& name, float fontSize) {
		m_ttfFont = TTF_OpenFont(name.c_str(), fontSize);
		if (m_ttfFont == nullptr) {
			std::cerr << "Could not load font: " << name << std::endl;
			return false;
		}

		return true;
	}

	bool Font::ChangeFontSize(const std::string& name, float newFontSize)
	{
		if (m_ttfFont == nullptr)
		{
			std::cerr << "You gotta have a font before you change the size bozo" << std::endl;
			return false;
		}
		
		return Load(name, newFontSize);

	}

}