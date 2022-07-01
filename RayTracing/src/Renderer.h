#ifndef  _RENDERER_H_
#define _RENDERER_H_

#include "Walnut/Image.h"
#include <glm/glm.hpp>

#include <memory>

class Renderer
{
public:
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render();
	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }
private:
	uint32_t PerPixel(glm::vec2 coordinate);

	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
};

#endif // ! _RENDERER_H_

