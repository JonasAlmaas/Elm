project "msdf-atlas-gen"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"msdf-atlas-gen/msdf-atlas-gen/**.h",
		"msdf-atlas-gen/msdf-atlas-gen/**.hpp",
		"msdf-atlas-gen/msdf-atlas-gen/**.cpp"
	}

	includedirs {
		"_config",
		"msdf-atlas-gen/artery-font-format",
		"msdf-atlas-gen/msdf-atlas-gen",
		"msdf-atlas-gen/msdfgen",
		"msdf-atlas-gen/msdfgen/include"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	links {
		"msdfgen"
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
