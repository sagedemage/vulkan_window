@RD /S /Q "build"
cmake -T ClangCL -A x64 -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
