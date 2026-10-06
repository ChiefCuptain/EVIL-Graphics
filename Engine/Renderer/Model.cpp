#include "pch.h"
#include "Renderer/Model.h"

namespace nu
{
	void Model::CalculateRadius()
	{
		m_radius = 0.0f;

		// Check for mesh radius

		for (const auto& mesh : m_meshes)
		{
			// Get mesh radius
			float radius = mesh.GetRadius();
			// If the mesh's radius is greater than the model's radius, set the model's radius to the mesh's radius
			if (radius > m_radius)
			{
				m_radius = radius;
			}
		}

	}
}