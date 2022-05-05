cmake_minimum_required(VERSION 3.13)
project(remu)

file(GLOB_RECURSE SRC "src/*.cpp")

if (MSVC)
    add_compile_options(/std:c++17)
else()
    add_compile_options(-std=c++17)
endif ()


add_executable(remu ${SRC})

if (MSVC)
    # FFTW3
    find_package(SDL2 CONFIG REQUIRED)
    target_link_libraries(remu PUBLIC SDL2::SDL2 SDL2::SDL2main)

else()
    find_package(PkgConfig)

    pkg_check_modules(SDL2 REQUIRED sdl2)

    target_include_directories(remu PUBLIC
        ${SDL2_INCLUDE_DIRS}
    )
    
    target_link_directories(remu PUBLIC
        ${SDL2_LIBRARY_DIRS}
    )

    target_link_libraries(remu PUBLIC
        ${SDL2_LIBRARIES}
    )
endif ()

# cmake .. "-DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake"