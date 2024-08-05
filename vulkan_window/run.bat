:: Clean build
MSBuild.exe ..\vulkan_window.sln /p:CLToolExe=clang-cl -target:Clean

:: Build and run the program
MSBuild.exe ..\vulkan_window.sln /p:CLToolExe=clang-cl
..\x64\Debug\vulkan_window.exe

