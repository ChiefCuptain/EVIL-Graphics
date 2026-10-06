#pragma once

namespace nu
{
	class Timer
	{
	public:
		Timer(float length) : m_length{ length }, m_elapsed{(m_length > 0.0f) ? false : true} {}

		void Decrement(float dt) { m_length -= dt; IsElapsed(); }

		void SetTimer(float length) { m_length = length; }

		bool IsElapsed() { m_elapsed = m_length < 0.0f; return m_elapsed; }
	private:

		float m_length = 0.0f;
		bool m_elapsed = false;
	};
}