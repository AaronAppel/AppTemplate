project "bimg"
	kind "StaticLib"
	location ""

	files
	{
		"include/bimg/*.h",
		"src/image.cpp",
		"src/image_gnf.cpp",
		"src/*.h",
		"src/*.cpp",
		"3rdparty/astc-encoder/source/*",
		"3rdparty/tinyexr/deps/miniz/miniz*",
	}

	includedirs
	{
		-- "include",
		-- "3rdparty/astc-encoder/include",
		-- "3rdparty/tinyexr/deps/miniz",
		-- "../bx/include/compat/msvc",
		-- "../bx/include",
		
		"%{wks.location}/Libraries/bx/include",
		"%{wks.location}/Libraries/bx/include/compat/msvc",
		
		"%{wks.location}/Libraries/%{prj.name}/include",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty/astc-codec",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty/astc-codec/include",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty/astc-encoder/include",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty/iqa/include",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty/tinyexr/deps/miniz",
	}

	defines
	{
		-- "BX_CONFIG_DEBUG=1",
		-- "__STDC_LIMIT_MACROS",
		-- "__STDC_FORMAT_MACROS",
		-- "__STDC_CONSTANT_MACROS",
		-- "_DEBUG",
		-- "WIN32",
		-- "_WIN32",
		-- "_HAS_EXCEPTIONS=0",
		-- "_SCL_SECURE=0",
		-- "_SECURE_SCL=0",
		-- "_SCL_SECURE_NO_WARNINGS",
		-- "_CRT_SECURE_NO_WARNINGS",
		-- "_CRT_SECURE_NO_DEPRECATE",
		
		"_DEBUG",
		"BX_CONFIG_DEBUG=1",
		"_HAS_EXCEPTIONS=0",
	}
	
	links { }
