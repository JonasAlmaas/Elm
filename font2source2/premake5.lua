project "font2source2"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++23"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.hpp",
		"src/**.cpp",

		"%{IncludeDir.stb}/stb_image.cpp",
		"%{IncludeDir.stb}/stb_image_write.cpp",
	}

	includedirs {
		"src",

		"%{IncludeDir.vendor_config}",

		"%{IncludeDir.msdf_atlas_gen}",
		"%{IncludeDir.msdfgen}",
		"%{IncludeDir.stb}",
	}

	links {
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

	filter "configurations:Release"
		defines "ELM_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "ELM_DIST"
		runtime "Release"
		optimize "on"
