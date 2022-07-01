#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

#include "Renderer.h"

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
		auto image = m_renderer.GetFinalImage();
		if (image)
			ImGui::Image(image->GetDescriptorSet(), { static_cast<float>(image->GetWidth()), static_cast<float>(image->GetHeight())}, 
				ImVec2{0, 1},  ImVec2{1, 0}); // flip the image
		ImGui::End();

		ImGui::PopStyleVar();
	}

	void Render() {
		Timer timer;

		// Renderer resize
		m_renderer.OnResize(m_ViewportWidth, m_ViewportHeight);

		// Renderer render
		m_renderer.Render();

		m_ElapsedTime = timer.ElapsedMillis();
	}
private:
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	Renderer m_renderer;
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