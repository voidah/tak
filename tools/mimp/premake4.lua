-- http://industriousone.com/topic/newplatform
function newplatform(plf)
    local name = plf.name
    local description = plf.description
 
    -- Register new platform
    premake.platforms[name] = {
        cfgsuffix = "_"..name,
        iscrosscompiler = true
    }
 
    -- Allow use of new platform in --platfroms
    table.insert(premake.option.list["platform"].allowed, { name, description })
    table.insert(premake.fields.platforms.allowed, name)
 
    -- Add compiler support
    -- gcc
    premake.gcc.platforms[name] = plf.gcc
    --other compilers (?)
end
 
function newgcctoolchain(toolchain)
    newplatform {
        name = toolchain.name,
        description = toolchain.description,
        gcc = {
            cc = toolchain.prefix .. "gcc",
            cxx = toolchain.prefix .. "g++",
            ar = toolchain.prefix .. "ar",
            cppflags = "-MMD " .. toolchain.cppflags
        }
    }
end

newplatform {
    name = "mingw",
    description = "Mingw Windows cross-compiler",
    gcc = {
        cc = "/usr/bin/i486-mingw32-gcc",
        cxx = "/usr/bin/i486-mingw32-g++",
        cppflags = "-DMINGW32"
    }
}
 


solution "mimp"
   configurations { "Debug", "Release" }
   platforms { "native", "mingw" }
   language "C++"
   targetdir "bin"

   configuration { "native", "linux" }
      links { "assimp" }
 
   configuration "windows"
      -- includedirs { "tak/external/vstudio2k10/glew-1.7.0/include", "tak/external/vstudio2k10/SFML-2.0-rc/include", "tak/external/vstudio2k10/DevIL-SDK-x86-1.7.8/include" }
	  -- libdirs { "tak/external/vstudio2k10/glew-1.7.0/lib", "tak/external/vstudio2k10/SFML-2.0-rc/lib", "tak/external/vstudio2k10/DevIL-SDK-x86-1.7.8/lib" }
      -- links { "opengl32", "glu32", "glew32", "DevIL", "ILU", "ILUT", "sfml-graphics", "sfml-main", "sfml-system", "sfml-window" }

      configuration "mingw"
      -- Does not work at the moment
      -- links { "m", "GL", "GLU", "pthread", "sndfile", "openal", "GLEW", "sfml-audio", "sfml-graphics", "sfml-window", "sfml-system", "Xrandr", "IL", "ILU", "X11", "dl" }
      -- includedirs { "tak/external/mingw/glew/include", "tak/external/mingw/sfml2/include" }
      -- libdirs { "tak/external/mingw/glew/lib", "tak/external/mingw/sfml2/lib" }

   project "mimp"
      kind "ConsoleApp"
      files { "*.h", "*.cpp" }
      location ("build/" .. iif(_ACTION ~= nil, _ACTION, ""))
      -- links { "tak" }
      -- includedirs { "tak" }
      uuid "3DDD58D6-F7FD-1B4E-5AF8-DE509FA64237"
 
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }

