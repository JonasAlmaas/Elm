#include "sandbox_app.h"
#include <stdio.h>

class ExampleLayer : public elm::Layer
{
public:
	ExampleLayer()
		: Layer("ExampleLayer")
	{
	}

	void on_update(void) override
	{
		ELM_TRACE("{0}::on_update()", get_name());
	}

	void on_event(elm::Event& e) override
	{
		ELM_TRACE("{0}::on_event({1})", get_name(), e);
	}
};

Sandbox::Sandbox()
{
	push_layer(new ExampleLayer());
	push_overlay(new elm::ImGuiLayer());
}

Sandbox::~Sandbox()
{
}
