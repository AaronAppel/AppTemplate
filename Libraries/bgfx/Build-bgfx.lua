project "bgfx"
	kind "StaticLib"
	location ""
	
	files
	{
		"include/bgfx/**.h",
		"src/*.cpp",
		"src/*.h",
		"3rdparty/meshoptimizer/src/*codec.cpp",
		"3rdparty/dear-imgui/*",
	}
	
	excludes
	{
		"src/amalgamated.cpp",
	}

	includedirs
	{
		"include",
		"3rdparty",
		"3rdparty/directx-headers/include/directx",
		"3rdparty/dxsdk/include",
		"3rdparty/khronos",
		
		"%{wks.location}/Libraries/bx/include",
		"%{wks.location}/Libraries/bx/include/compat/msvc",
		"%{wks.location}/Libraries/bimg/include",
	}

	defines
	{
		"_DEBUG",
		"BX_CONFIG_DEBUG=1",
		"__STDC_FORMAT_MACROS",
		"_CRT_SECURE_NO_WARNINGS",
		"_HAS_EXCEPTIONS=0",
	}
	
	links
	{
		"bimg",
		"bx",
	}

	filter "configurations:Debug"
		defines { "BX_CONFIG_DEBUG=1", }
		
	filter "configurations:Release"
		-- defines { "BX_CONFIG_DEBUG=0", }
