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

	group "Dependencies"
	include "Elm/vendor/glad"
	include "Elm/vendor/glfw"
	include "Elm/vendor/imgui"
	group ""

	group "Dependencies/msdf"
	include "Elm/vendor/msdf-atlas-gen"
	include "Elm/vendor/msdfgen"
	include "Elm/vendor/freetype"
	group ""
