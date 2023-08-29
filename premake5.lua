-- premake5.lua
workspace "goofy ahh half functioning ray tracer"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "goofy ahh half functioning ray tracer"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "goofy ahh half functioning ray tracer"