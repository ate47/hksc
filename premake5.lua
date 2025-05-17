newoption {
   trigger = "ci-build",
   description = "CI build define"
}

workspace "hksc"
    startproject "hksc"
    location "./build"
    configurations { 
        "Debug",
        "Release"
    }

    architecture "x86_64"
    platforms "x64"

    filter { "options:ci-build" }
        defines { "CI_BUILD" }
    
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "Size"
        
    filter {} -- Reset filters

    
project "hksc"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "%{wks.location}/bin/"
    objdir "%{wks.location}/obj/"

    targetname "hksc"
    
    files {
        "./src/**.hpp",
        "./src/**.h",
        "./src/**.c",
        "./src/**.def"
    }

    includedirs {
        "src"
    }

    vpaths {
        ["*"] = "*"
    }