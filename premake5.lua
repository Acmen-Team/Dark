workspace "Dark"
	architecture "x86_64"
	startproject "Dark-Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder(solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Dark/vendor/GLFW/include"
IncludeDir["Glad"] = "Dark/vendor/Glad/include"
IncludeDir["ImGui"] = "Dark/vendor/imgui"
IncludeDir["glm"] = "Dark/vendor/glm"
IncludeDir["stduuid"] = "Dark/vendor/stduuid"
IncludeDir["stb_image"] = "Dark/vendor/stb_image"
IncludeDir["tinyobjloader"] = "Dark/vendor/tinyobjloader"
IncludeDir["Vulkan"] = "Dark/vendor/Vulkan/include"
IncludeDir["entt"] = "Dark/vendor/entt/include"
IncludeDir["ImGuizmo"] = "Dark/vendor/ImGuizmo"
IncludeDir["IconFont"] = "Dark/vendor/IconFontCppHeaders"
IncludeDir["GSL"] = "Dark/vendor/GSL/include/"
IncludeDir["yaml_cpp"] = "Dark/vendor/yaml_cpp/include"
IncludeDir["OpenXR"] = "Dark/vendor/OpenXR/include"
IncludeDir["OpenAL"] = "Dark/vendor/OpenAL-Soft/include"
IncludeDir["minimp3"] = "Dark/vendor/minimp3"
IncludeDir["Steamworks"] = "Dark/vendor/Steamworks"

group "Dependencies"
	include "Dark/vendor/GLFW"
	include "Dark/vendor/Glad"
	include "Dark/vendor/imgui"
	include "Dark/vendor/yaml_cpp"
	include "Dark/vendor/OpenXR"
	include "Dark/vendor/OpenAL-Soft"
group ""

project "Dark"
	location "Dark"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "dkpch.h"
	pchsource "Dark/src/dkpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stduuid/**.h",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/tinyobjloader/**.h",
		"%{prj.name}/vendor/entt/include/entt.hpp",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp",
		"%{prj.name}/vendor/IconFontCppHeaders/**.h",
		"%{prj.name}/vendor/Vulkan/include/vulkan/**.h",
		"%{prj.name}/vendor/Vulkan/include/vulkan/**.cpp",
		"%{prj.name}/vendor/GSL/include/gsl/**",
		"%{prj.name}/vendor/Steamworks/steam/**.h",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"AL_LIBTYPE_STATIC"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.OpenXR}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stduuid}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.tinyobjloader}",
		"%{IncludeDir.Vulkan}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.IconFont}",
		"%{IncludeDir.GSL}",
		"%{IncludeDir.OpenAL}",
		"%{IncludeDir.minimp3}",
		"%{IncludeDir.Steamworks}",
	}

	libdirs { "Dark/vendor/Vulkan/Lib", "Dark/vendor/Steamworks/lib" }

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"XInput.lib",
		"vulkan-1.lib",
		"yaml-cpp",
		"OpenXR-Loader",
		"OpenAL-Soft",
		"steam_api64.lib"
	}

	filter "files:Dark/vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"DK_PLATFORM_WINDOWS",
			-- "DK_BUILD_DLL",
			"DK_ENABLE_ASSERTS",
			-- "DK_RENDER_VULKAN"
		}

	filter "configurations:Debug"
		defines "DK_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "DK_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "DK_Dist"
		runtime "Release"
		optimize "On"

project "Dark-Editor"
	location "Dark-Editor"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Dark/vendor/spdlog/include",
		"Dark/src",
		"Dark/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.stduuid}",
		"%{IncludeDir.IconFont}",
		"%{IncludeDir.GSL}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.OpenAL}",
		"%{IncludeDir.minimp3}",
	}

	links
	{
		"Dark"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"DK_PLATFORM_WINDOWS",	
			"AL_LIBTYPE_STATIC",
		}

	filter "configurations:Debug"
		defines "DK_DEBUG"
		runtime "Debug"
		kind "ConsoleApp"
		symbols "On"

	filter "configurations:Release"
		defines "DK_RELEASE"
		runtime "Release"
		kind "WindowedApp"
		optimize "On"

	filter "configurations:Dist"
		defines "DK_Dist"
		runtime "Release"
		kind "WindowedApp"
		entrypoint "mainCRTStartup"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Dark/vendor/spdlog/include",
		"Dark/src",
		"%{IncludeDir.ImGui}",
		-- "Dark/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stduuid}",
		"%{IncludeDir.GSL}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.OpenAL}",
		"%{IncludeDir.minimp3}",
	}

	links
	{
		"Dark"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"DK_PLATFORM_WINDOWS",	
		}

	filter "configurations:Debug"
		defines "DK_DEBUG"
		runtime "Debug"
		kind "ConsoleApp"
		symbols "On"

	filter "configurations:Release"
		defines "DK_RELEASE"
		runtime "Release"
		kind "WindowedApp"
		optimize "On"

	filter "configurations:Dist"
		defines "DK_Dist"
		runtime "Release"
		kind "WindowedApp"
		entrypoint "mainCRTStartup"
		optimize "On"