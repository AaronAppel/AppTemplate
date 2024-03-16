project "App"
	kind "ConsoleApp"
	location ""
	
	defines	{ }	
	debugargs { }
	postbuildcommands { }

	files
	{
		"Source/**.h",
		"Source/**.cpp",
		"Source/**.hpp",
		"Source/**.c",
	}
	
	excludes
	{
		"Source/main_working.cpp",
	}

	includedirs
	{
		"%{wks.location}/Libraries/", -- glfw, maybe others
		"%{wks.location}/Libraries/bgfx/include",
		"%{wks.location}/Libraries/bgfx/3rdparty",
		"%{wks.location}/Libraries/bx/include",
		"%{wks.location}/Libraries/bx/include/compat/msvc",
		"%{wks.location}/Libraries/bimg/include",
		
		"%{wks.location}/Libraries/bgfxApp/common/entry",
	}

	links
	{
		"bgfxApp",
	}

	filter "system:windows"
		systemversion "latest"
		defines { "WINDOWS" }

	filter "configurations:Debug"
		defines { LibraryDefines, "ENTRY_CONFIG_USE_GLFW", "BX_CONFIG_DEBUG=1", }
		runtime "Debug"
		symbols "on"
		optimize "off"

	filter "configurations:Release"
		defines { LibraryDefines, }
		runtime "Release"
		symbols "off"
		optimize "on"

	filter "configurations:Retail"
		defines { LibraryDefines, }
		runtime "Release"
		symbols "off"
		optimize "on"
