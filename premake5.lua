include "./vendor/premake/premake_customization/solution_items.lua"
include "dependencies.lua"

workspace "Elm"
	architecture "x86_64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist",
	}

	solution_items {
		".editorconfig"
	}

	flags {
		"MultiProcessorCompile"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	group "Core"
		include "Elm"
	group ""

	include "Sandbox"

	group "Tools"
		include ""
	group ""

	group "Dependencies"
		include "Elm/vendor/glfw"
	group ""
