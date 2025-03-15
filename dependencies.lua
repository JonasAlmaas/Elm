VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}

IncludeDir["entt"] = "%{wks.location}/Elm/vendor/entt/include"
IncludeDir["glad"] = "%{wks.location}/Elm/vendor/glad/include"
IncludeDir["glm"] = "%{wks.location}/Elm/vendor/glm"
IncludeDir["glfw"] = "%{wks.location}/Elm/vendor/glfw/include"
IncludeDir["imgui"] = "%{wks.location}/Elm/vendor/imgui"
IncludeDir["msdf_atlas_gen"] = "%{wks.location}/Elm/vendor/msdf-atlas-gen/msdf-atlas-gen"
IncludeDir["msdfgen"] = "%{wks.location}/Elm/vendor/msdf-atlas-gen/msdfgen"
IncludeDir["spdlog"] = "%{wks.location}/Elm/vendor/spdlog/include"
IncludeDir["stb"] = "%{wks.location}/Elm/vendor/stb"
IncludeDir["tinyobjloader"] = "%{wks.location}/Elm/vendor/tinyobjloader"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
