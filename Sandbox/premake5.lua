project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++23"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp",
	}

	includedirs {
		"src",

		"../Elm/include",
		"../Elm/src",

		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.stb}",
	}

	links {
		"Elm"
	}

	filter "system:windows"
		systemversion "latest"
		buildoptions { "/utf-8" }
		defines "ELM_PLATFORM_WINDOWS"

	filter "configurations:Debug"
		defines "ELM_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ELM_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "ELM_DIST"
		runtime "Release"
		optimize "on"
