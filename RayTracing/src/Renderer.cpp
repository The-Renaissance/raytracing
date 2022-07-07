#include <Walnut/Random.h>

#include "Renderer.h"

void Renderer::Render()
{
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); ++y)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); ++x)
		{
			glm::vec2 coord = { static_cast<float>(x) / m_FinalImage->GetWidth(),  static_cast<float>(y) / m_FinalImage->GetHeight() };
			coord = coord * 2.0f - 1.0f;
			m_ImageData[y * m_FinalImage->GetWidth() + x] = PerPixel(coord);

		}
	}
	m_FinalImage->SetData(m_ImageData);
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		// No resize necessary
		if (m_FinalImage->GetHeight() == height && m_FinalImage->GetWidth() == width)
			return;
		m_FinalImage->Resize(width, height);
	} else {
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
}

uint32_t Renderer::PerPixel(glm::vec2 coordinate)
{
	
	glm::vec3 ray_origin{ 0.0f, 0.0f, 2.0f };
	glm::vec3 ray_direction{ coordinate.x, coordinate.y, -1.0f };
	glm::vec3 sphere_origin{ 0.5f, 0.5f, -1.0f };
	float radius = 0.5f;

	// b*bt^2 + 2b*(a-o)t + a*a-2(a*o) + o*o - r^2 = 0
	// where
	// a = ray origin
	// b = ray direction
	// o = sphere origin
	// r = radius
	// t = hit distance

	float a = glm::dot(ray_direction, ray_direction);
	float b = 2 * glm::dot(ray_direction, ray_origin - sphere_origin);
	float c = glm::dot(ray_origin, ray_origin) - 2 * glm::dot(ray_origin, sphere_origin)
		+ glm::dot(sphere_origin, sphere_origin) - radius * radius;

	// b ^ 2 - 4ac
	float discriminant = b * b - 4 * a * c;
	
	if (discriminant >= 0.0f)
		return 0xffff00ff;
	return 0xff000000;
}
