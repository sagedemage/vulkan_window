:: Clean build
MSBuild.exe ..\vulkan_window.sln /p:PlatformToolset=ClangCL -target:Clean

:: Build and run the program
MSBuild.exe ..\vulkan_window.sln /p:PlatformToolset=ClangCL
..\x64\Debug\vulkan_window.exe

