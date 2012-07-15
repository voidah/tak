solution "bundleelf"
   configurations { "Debug", "Release" }
   language "C++"

   configuration { "native", "linux" }
      links { "assimp" }

   project "bundleelf"
      kind "ConsoleApp"
      files { "*.h", "*.cpp" }
      location ("build/" .. iif(_ACTION ~= nil, _ACTION, ""))
 
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }

