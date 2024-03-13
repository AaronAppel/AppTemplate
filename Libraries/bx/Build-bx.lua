project "bx"
	kind "StaticLib"
	location ""

	files
	{
		"include/%{prj.name}/*.h",
		"include/%{prj.name}/inline/*.inl",
		"src/*.cpp"
	}
	
	excludes
	{
		"src/amalgamated.cpp",
	}

	includedirs
	{
		"%{wks.location}/Libraries/%{prj.name}/include",
		"%{wks.location}/Libraries/%{prj.name}/include/compat/msvc",
		"%{wks.location}/Libraries/%{prj.name}/3rdparty",
	}

	defines
	{
		-- "__STDC_FORMAT_MACROS",
		
		"_DEBUG",
		"__STDC_FORMAT_MACROS",
		"BX_CONFIG_DEBUG=1",
		"_CRT_SECURE_NO_WARNINGS",
		"_HAS_EXCEPTIONS=0",
	}
	
	links {	}
	
	filter "configurations:Debug"
		defines "BX_CONFIG_DEBUG=1"
		
	filter "configurations:Release"
		defines "BX_CONFIG_DEBUG=0"
		
	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"