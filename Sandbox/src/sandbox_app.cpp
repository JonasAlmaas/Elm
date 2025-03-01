#include "sandbox_app.h"
#include <imgui.h>
#include <stdio.h>

class example_layer : public elm::layer
{
public:
	example_layer(void)
		: layer("ExampleLayer")
	{
	}

	virtual void on_update(void) override
	{
		//ELM_TRACE("{0}::on_update()", get_name());
	}

	virtual void on_event(elm::event& e) override
	{
		//ELM_TRACE("{0}::on_event({1})", get_name(), e);
	}

	virtual void on_imgui_render(void)
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello world");
		ImGui::End();
	}

};

sandbox::sandbox()
{
	push_layer(new example_layer());
}

sandbox::~sandbox()
{
}
