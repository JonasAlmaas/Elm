project "Elm"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp",

		"include/**.h",

		"%{IncludeDir.stb}/stb_image.cpp",
		"%{IncludeDir.tinyobjloader}/tiny_obj_loader.cpp",
	}

	includedirs {
		"src",
		"resources",

		"%{IncludeDir.vendor_config}",

		"%{IncludeDir.entt}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.msdf_atlas_gen}",
		"%{IncludeDir.msdfgen}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.tinyobjloader}",
		"%{IncludeDir.VulkanSDK}",
	}

	links {
		"glad",
		"GLFW",
		"imgui",
		"msdf-atlas-gen",
	}

	filter "system:windows"
		systemversion "latest"
		buildoptions { "/utf-8" }
		defines "ELM_PLATFORM_WINDOWS"

	filter "configurations:Debug"
		defines "ELM_DEBUG"
		runtime "Debug"
		symbols "on"

		links {
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}

	filter "configurations:Release"
		defines "ELM_RELEASE"
		runtime "Release"
		optimize "on"

		links {
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

	filter "configurations:Dist"
		defines "ELM_DIST"
		runtime "Release"
		optimize "on"

		links {
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}
