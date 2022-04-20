workspace "ShaderApp"
    architecture "x86_64"
    startproject "Editor"
    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
workspacedir = os.getcwd()

group "Dependencies"
    include "GLFW"
    include "Glad"
    include "imgui"
group ""
