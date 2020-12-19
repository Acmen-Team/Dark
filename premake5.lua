workspace "Dark"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Dark"
	location "Dark"
	kind "SharedLib"
	language "C++"
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
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"DK_PLATFORM_WINDOWS",
			"DK_BUILD_DLL"		
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
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

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
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
		"Dark/src;"
	}

	links
	{
		"Dark"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"DK_PLATFORM_WINDOWS",	
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