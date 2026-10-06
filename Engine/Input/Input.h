#pragma once
#include "Math/Vector2.h"
#include <vector>


namespace nu
{
	class Input
	{
	public:
		enum MouseButton
		{
			Left = 1,
			Middle = 2,
			Right = 3,
			X1 = 4,
			X2 = 5
		};

	public:
		bool Initialize();
		void Quit();

		void Update();

		bool GetKeyDown(int key) const{ return m_keyStates.at(key); }
		bool GetPrevKeyDown(int key) const{ return m_prevKeyStates.at(key); }
		bool GetKeyPressed(int key) const { return !m_prevKeyStates.at(key) && m_keyStates.at(key); }
		bool GetKeyReleased(int key) const { return m_prevKeyStates.at(key) && !m_keyStates.at(key); }

		bool GetMouseDown(MouseButton button) const { return m_buttonStates & GetButtonBit(button); }
		bool GetPrevMouseDown(MouseButton button) const { return m_prevButtonStates & GetButtonBit(button); }
		bool GetMousePressed(MouseButton button) const { return !GetPrevMouseDown(button) && GetMouseDown(button); }
		bool GetMouseReleased(MouseButton button) const { return GetPrevMouseDown(button) && !GetMouseDown(button); }

		Vector2 GetMousePosition() const { return m_mousePosition; }

	private:
		uint32_t GetButtonBit(MouseButton button) const;

	private:
		// Keyboard
		std::vector<bool> m_keyStates;
		std::vector<bool> m_prevKeyStates;

		// Mouse
		uint32_t m_buttonStates = 0;
		uint32_t m_prevButtonStates = 0;

		Vector2 m_mousePosition = { 0, 0 };
	};
}