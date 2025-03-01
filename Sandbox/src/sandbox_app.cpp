#include "sandbox_app.h"
#include <imgui.h>
#include <stdio.h>

class ExampleLayer : public elm::Layer
{
public:
	ExampleLayer()
		: Layer("ExampleLayer")
	{
	}

	virtual void on_update(void) override
	{
		//ELM_TRACE("{0}::on_update()", get_name());
	}

	virtual void on_event(elm::Event& e) override
	{
		//ELM_TRACE("{0}::on_event({1})", get_name(), e);
	}

	virtual void on_imgui_render(void)
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello world");
		ImGui::End();;
	}
};

Sandbox::Sandbox()
{
	push_layer(new ExampleLayer());
}

Sandbox::~Sandbox()
{
}
