# Unix Makefiles version with Clang and LLVM
Remove-Item -r -fo "build"
$env:CC="C:\Program Files\LLVM\bin\clang.exe"
$env:CCX="C:\Program Files\LLVM\bin\clang++.exe"
cmake -G "Unix Makefiles" -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -D_CMAKE_TOOLCHAIN_PREFIX=llvm-
