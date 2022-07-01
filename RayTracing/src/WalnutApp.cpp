#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

#include "Geometry.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last Render: %.3fms", m_ElapsedTime);
		if (ImGui::Button("Render")) {
			Render();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });

		ImGui::Begin("Viewport");
		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;
		Render();
		ImGui::Image(m_Image->GetDescriptorSet(), { static_cast<float>(m_Image->GetWidth()), static_cast<float>(m_Image->GetHeight()) });
		ImGui::End();

		ImGui::PopStyleVar();
	}

	void Render() {
		Timer timer;

		if (!m_Image || m_ViewportWidth != m_Image->GetWidth() || m_ViewportHeight != m_Image->GetHeight()) {
			m_Image = std::make_shared<Image>(m_ViewportWidth, m_ViewportHeight, ImageFormat::RGBA);
			delete[] m_ImageData;
			m_ImageData = new uint32_t[m_ViewportWidth * m_ViewportHeight];
		}

		auto upperLeftCorner = m_ViewportCenter + glm::vec3{ 0.0f, -(m_ViewportWidth / 2.0f), m_ViewportHeight / 2.0f };
		for (size_t i = 0; i < m_ViewportHeight * m_ViewportWidth; ++i)
		{
			auto coordinate = upperLeftCorner + glm::vec3{ 0.0f, static_cast<float>(i % m_ViewportWidth), -static_cast<float>(i / m_ViewportWidth) };
			if (is_sphere_hit(Ray{ m_camera, glm::normalize(coordinate - m_camera) }, m_sphere))
			{
				m_ImageData[i] = 0x000000ff; // Red ff0000
			} else {
				m_ImageData[i] = 0x00ffffff; // White
			}
			m_ImageData[i] |= 0xff000000;
		}

		m_Image->SetData(m_ImageData);

		m_ElapsedTime = timer.ElapsedMillis();
	}
private:
	std::shared_ptr<Image> m_Image;
	uint32_t* m_ImageData = nullptr;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	// Scene
	Sphere m_sphere{ glm::vec3{10, 0, 0}, 495.f };
	glm::vec3 m_camera = glm::vec3{ -500, 0, 0 }, m_ViewportCenter = glm::vec3{ -490,0,0 };

	float m_ElapsedTime = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Random::Init();

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}