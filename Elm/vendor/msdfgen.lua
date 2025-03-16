project "msdfgen"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"msdf-atlas-gen/msdfgen/core/**.h",
		"msdf-atlas-gen/msdfgen/core/**.hpp",
		"msdf-atlas-gen/msdfgen/core/**.cpp",
		"msdf-atlas-gen/msdfgen/ext/**.h",
		"msdf-atlas-gen/msdfgen/ext/**.hpp",
		"msdf-atlas-gen/msdfgen/ext/**.cpp",
		"msdf-atlas-gen/msdfgen/lib/**.cpp",
		"msdf-atlas-gen/msdfgen/include/**.h"
	}

	includedirs {
		"_config",
		"msdf-atlas-gen/msdfgen/include",
		"freetype/include"
	}

	defines {
		"MSDFGEN_USE_CPP11"
	}

	links {
		"freetype"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
		symbols "off"
