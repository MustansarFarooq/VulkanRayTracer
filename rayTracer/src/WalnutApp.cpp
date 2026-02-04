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
		Material& pinkSphere = m_scene.Materials.emplace_back();
		pinkSphere.Albedo = { 1.0f,0.0f,1.0f };
		pinkSphere.Roughness = 0.0f;

		Material& blueSphere = m_scene.Materials.emplace_back();
		blueSphere.Roughness = 1.0f;
		blueSphere.Albedo = { 0.2f, 0.3f, 1.0f };
		{
			Sphere sphere;
			sphere.Position = { 0.0f,0.0f,0.0f };
			sphere.radius = 1.0f;
			sphere.MaterialIndex = 0;
			m_scene.Spheres.push_back(sphere);
		}

		{
			Sphere sphere;
			sphere.Position = { 0.0f,-101.0f,0.0f };
			sphere.radius = 100.0f;
			sphere.MaterialIndex = 1;
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
				ImGui::DragInt("Material",&sphere.MaterialIndex,1.0f,0,(int)m_scene.Materials.size() - 1);

				ImGui::Separator();
				ImGui::PopID();
			}
			for (size_t i = 0; i < m_scene.Materials.size(); i++)
			{
				ImGui::PushID(i);
				Material& material = m_scene.Materials[i];
				ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo));
				ImGui::DragFloat("Mettalic", &material.Metallic,0.05f,0.0f,1.0f);
				ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.0f, 1.0f);
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
	spec.Name = "Raytracer";

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
