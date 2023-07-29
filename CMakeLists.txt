cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 23)
project(Squishy VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
if (MSVC)
    add_compile_options(/W4 /WX)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /fp:fast /arch:AVX2")
else()
    add_compile_options(-Wall -Wextra -Wno-unused-parameter -Werror)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ffast-math -march=native")
endif()

include(CTest)
enable_testing()

file(GLOB_RECURSE all_files CONFIGURE_DEPENDS "src/*.cpp" "include/*.h")
add_executable(${PROJECT_NAME} main.cpp ${all_files})
target_include_directories(${PROJECT_NAME} PRIVATE include)

add_subdirectory(glt)
target_link_libraries(${PROJECT_NAME} PRIVATE glt)

find_package(benchmark CONFIG REQUIRED)
target_link_libraries(Squishy PRIVATE benchmark::benchmark benchmark::benchmark_main)
