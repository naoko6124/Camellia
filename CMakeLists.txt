cmake_minimum_required (VERSION 3.8)

project (Camellia)
file (GLOB_RECURSE CamelliaSrc Camellia/**.*)
add_executable (Camellia WIN32 ${CamelliaSrc})
target_include_directories (Camellia PUBLIC Camellia $ENV{DXSDK_DIR}Include)
target_link_directories (Camellia PUBLIC $ENV{DXSDK_DIR}Lib/x64)
target_link_libraries (Camellia PUBLIC d3d9 d3dx9)

project (CATool)
add_executable (CATool CATool/main.cpp)