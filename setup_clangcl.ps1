Remove-Item -r -fo "build"
cmake -T ClangCL -A x64 -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1
