-- premake5.lua
workspace "Darkstorm"
   configurations { "Debug", "Release" }

   platforms { "x32"}

project "Darkstorm"
   kind "SharedLib"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   files { "**.h", "**.cpp" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      flags { "Symbols" }
	  links { "segvcatch" }
	  buildoptions{ "-fpermissive -fPIC -std=c++14 -fnon-call-exceptions" }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
	  buildoptions{ "-fpermissive -fPIC -std=c++14 -fnon-call-exceptions -lsegvcatch" }
