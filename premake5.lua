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
IncludeDir["Vulkan"] = "Dark/vendor/Vulkan/include"
IncludeDir["entt"] = "Dark/vendor/entt/include"
IncludeDir["ImGuizmo"] = "Dark/vendor/ImGuizmo"
IncludeDir["IconFont"] = "Dark/vendor/IconFontCppHeaders"

group "Dependencies"
	include "Dark/vendor/GLFW"
	include "Dark/vendor/Glad"
	include "Dark/vendor/imgui"
group ""

project "Dark"
	location "Dark"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"

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
		"%{prj.name}/vendor/entt/include/entt.hpp",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp",
		"%{prj.name}/vendor/IconFontCppHeaders/**.h",
		"%{prj.name}/vendor/Vulkan/include/vulkan/**.h",
		"%{prj.name}/vendor/Vulkan/include/vulkan/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stduuid}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.Vulkan}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.IconFont}",
	}

	libdirs { "Dark/vendor/Vulkan/Lib" }

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"XInput.lib",
		"vulkan-1.lib"
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
	cppdialect "C++20"
	staticruntime "On"

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
		"%{IncludeDir.stduuid}",
		"%{IncludeDir.IconFont}",
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

project "Sandbox"
	location "Sandbox"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"

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
		"Dark/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stduuid}"
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