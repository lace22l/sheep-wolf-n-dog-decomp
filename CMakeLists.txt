cmake_minimum_required(VERSION 3.20)
project(SheepRaiderDecomp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 98)

add_executable(SheepRaiderDecomp WIN32 src/main.cpp
        resources/resource.h
        resources/embed_bmp.rc
        resources/launcher.rc
        src/bitmapUtils.cpp
        src/bitmapUtils.h)
target_include_directories(SheepRaiderDecomp PRIVATE resources)
target_link_libraries(SheepRaiderDecomp
        d3d9
        d3dx9
        dsetup
        dxguid
        user32
        gdi32
        kernel32
)
target_include_directories(SheepRaiderDecomp PRIVATE
    "C:\\DX9\\Include"
)
target_link_directories(SheepRaiderDecomp PRIVATE
        "C:\\DX9\\Lib\\x86"
)
#target_link_libraries(SheepRaiderDecomp
#        d3d8
#        d3dx8
#        dxguid
#)
target_compile_definitions(SheepRaiderDecomp PRIVATE
        WIN32_LEAN_AND_MEAN
        _CRT_SECURE_NO_WARNINGS
        _WIN32_WINNT=0x0601
)