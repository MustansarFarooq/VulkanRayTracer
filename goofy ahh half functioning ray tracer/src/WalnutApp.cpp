#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include "Renderer.h"
#include "Camera.h"
#include "glm/gtc/type_ptr.hpp"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer()
		: m_Camera(45.0f, 0.1f, 100.0f)
	{
		{
			Sphere sphere;
			sphere.Position = { 0.0f,0.0f,0.0f };
			sphere.radius = 0.5f;
			sphere.Albedo = { 1.0f,1.0f,1.0f };
			m_scene.Spheres.push_back(sphere);
		}

		{
			Sphere sphere;
			sphere.Position = { 1.0f,0.0f,-5.0f };
			sphere.radius = 1.5f;
			sphere.Albedo = { 1.0f,1.0f,1.0f };
			m_scene.Spheres.push_back(sphere); 
		}
	}
	
	virtual void OnUpdate(float ts) override 
	{
		m_Camera.OnUpdate(ts);
	}
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render time : %.3fms", m_lastRenderTime);
		if (ImGui::Button("Render")) 
		{
			Render();
		}
		ImGui::NewLine();
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Scene",ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (size_t i = 0; i < m_scene.Spheres.size(); i++)
			{
				ImGui::PushID(i);
				Sphere& sphere = m_scene.Spheres[i];
				ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
				ImGui::DragFloat("Radius", &sphere.radius, 0.1f);
				ImGui::ColorEdit3("Albedo", glm::value_ptr(sphere.Albedo));
				ImGui::Separator();
				ImGui::PopID();
			}
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");
		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;
		
		auto image = m_renderer.GetFinalImage();
		if (image)
		{
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(),(float)image->GetHeight() },
				ImVec2(0,1),ImVec2(1,0));
		}
		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render() 
	{
		Timer timer;
		m_renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_renderer.Render(m_scene,m_Camera);
		m_lastRenderTime = timer.ElapsedMillis();
	}

private:
	 
	
	Renderer m_renderer;
	Camera m_Camera;
	Scene m_scene;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	float m_lastRenderTime = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "goofy ahh half functioning ray tracer";

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