workspace "AppTemplate"

	-- Workspace specific settings
	startproject "App"
	location "../"

	-- Solution wide shared settings
	configurations { "Debug", "Release", "Retail" }
	flags { "MultiProcessorCompile" }
	rtti "Off"
	staticruntime "off" -- https://premake.github.io/docs/staticruntime
	language "C++"
	cppdialect "C++17"

	-- filter "configurations:*86"
		-- architecture "x86"
	-- filter "configurations:*64"
		-- architecture "x64"
		
	filter "configurations:*"
		architecture "x86" -- Only 32 bit supported for now
	
	OutputDir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"
	targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin/int/" .. OutputDir .. "/%{prj.name}")

	filter "system:windows"
		buildoptions { "/Zc:__cplusplus" }
		systemversion "latest"
		defines { "_QWINDOWS", "WIN32_LEAN_AND_MEAN", }
		
	filter "configurations:Debug"
		defines { "_QDebug", "WIN32_LEAN_AND_MEAN", } -- #TODO _QDEBUG should be all caps to match convention
		runtime "Debug"
		symbols "on"
		optimize "off"
		
	filter "configurations:Release"
		runtime "Release"
		symbols "off"
		optimize "on"
	
	group "Dependencies"
	include "../Libraries/bgfx/Build-bgfx.lua"
	include "../Libraries/bimg/Build-bimg.lua"
	include "../Libraries/bx/Build-bx.lua"
	include "../Libraries/bgfxApp/Build-bgfxApp.lua"
	group ""
	
	-- buildoptions { "/EHsc", "/Zc:preprocessor", } -- bx doesn't like "/EHsc", "/Zc:preprocessor"

	include "../App/App.lua"

