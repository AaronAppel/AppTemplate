include "Libraries.lua"

project "bgfxApp"
	kind "StaticLib"
	location ""

	files { "**.h", "**.cpp", }
	
	excludes
	{
		"QF_RendererBgfxExampleOriginal_prev.cpp",
		"QF_RendererBgfxExampleOriginal_HelloWorld.cpp",
		
		"common/entry/entry_android.cpp",
		"common/entry/entry_html5.cpp",
		"common/entry/entry_x11.cpp",
		"common/entry/entry_windows.cpp",
		"common/entry/entry_noop.cpp",
	}

	includedirs
	{
		"%{wks.location}/Libraries/", -- glfw, maybe others
		"%{wks.location}/Libraries/bgfx/include",
		"%{wks.location}/Libraries/bgfx/3rdparty",
		"%{wks.location}/Libraries/bx/include",
		"%{wks.location}/Libraries/bx/include/compat/msvc",
		"%{wks.location}/Libraries/bimg/include",
		"%{wks.location}/Libraries/%{prj.name}/common/entry",
	}
		
	links
	{
		"bgfx",
		"%{LibraryLib.glfw}",
	}

	defines { "ENTRY_CONFIG_USE_GLFW", }
	

	filter "system:windows"
		links { "psapi" }
		systemversion "latest"

	filter "configurations:Debug"
		defines { "BX_CONFIG_DEBUG=1", } -- Required by bgfx
		runtime "Debug"
		symbols "On"
		debugargs { }
