cmake_minimum_required(VERSION 3.22)
set(VCPKG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/glt/vcpkg")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON)
project(squishy)

add_custom_target(Assets)
# Create the assets folder
add_custom_command(TARGET Assets PRE_BUILD
COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:squishy>/assets)
# Make a copy in the executable directory
add_custom_command(TARGET Assets PRE_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/assets" $<TARGET_FILE_DIR:squishy>/assets)
# Make an additional copy in the binary dir in case a multi config generator is used
add_custom_command(TARGET Assets PRE_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/assets" ${PROJECT_BINARY_DIR}/assets)

file(GLOB_RECURSE all_files CONFIGURE_DEPENDS "src/*.cpp" "src/*.cc" "src/*.h")

list(FILTER all_files EXCLUDE REGEX "src/Ui/.*")

list(APPEND all_files 
  "src/Ui/home/homePage.cpp"
  "src/Ui/artifact/artifactPage.cpp"
  "src/Ui/artifact/artifactEditor.cpp"
  "src/Ui/artifact/artifactCard.cpp"
  "src/Ui/character/characterSelector.cpp"
  "src/Ui/utils/card.cpp"
  "src/Ui/utils/tag.cpp"
  "src/Ui/utils/statDisplay.cpp"
  "src/Ui/utils/editorItem.cpp"
  "src/Ui/utils/skillEntry.cpp"
)

add_library(squishy_lib ${all_files})
target_compile_features(squishy_lib PUBLIC cxx_std_23)
set_target_properties(squishy_lib PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(squishy_lib PUBLIC src)
add_dependencies(squishy_lib Assets)

if (MSVC)
    # target_compile_options(squishy_lib PUBLIC /fsanitize=address)
    target_compile_options(squishy_lib PUBLIC /std:c++latest /bigobj)
    target_compile_definitions(squishy_lib PUBLIC _DISABLE_STRING_ANNOTATION _DISABLE_VECTOR_ANNOTATION)
else()
    target_compile_options(squishy_lib PUBLIC -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Werror)
    # target_link_options(squishy_lib PUBLIC -fsanitize=address,undefined)
    # target_link_options(squishy_lib PUBLIC -fsanitize=undefined,thread)
endif()

find_package(fmt CONFIG REQUIRED)
target_link_libraries(squishy_lib PUBLIC fmt::fmt)

add_subdirectory(glt)
include_directories(glt/include)
target_link_libraries(squishy_lib PUBLIC glt)

include(FetchContent)

FetchContent_Declare(
  glaze
  GIT_REPOSITORY https://github.com/stephenberry/glaze.git
  GIT_TAG main
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glaze)
target_link_libraries(squishy_lib PUBLIC glaze::glaze)

find_package(nfd CONFIG REQUIRED)
target_link_libraries(squishy_lib PRIVATE nfd::nfd)

add_executable(squishy main.cpp)
target_link_libraries(squishy PRIVATE squishy_lib)

add_executable(squishyBench EXCLUDE_FROM_ALL benchmark.cpp)
target_link_libraries(squishyBench PRIVATE squishy_lib)

target_link_libraries(squishyBench PRIVATE glt)
find_package(benchmark CONFIG REQUIRED)
target_link_libraries(squishyBench PRIVATE benchmark::benchmark benchmark::benchmark_main)

file(GLOB_RECURSE all_test_files CONFIGURE_DEPENDS "tests/*.cc")
add_executable(squishyTest EXCLUDE_FROM_ALL ${all_test_files})
find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(squishyTest PRIVATE Catch2::Catch2WithMain)
target_link_libraries(squishyTest PRIVATE squishy_lib)

include(CTest)
include(Catch)
catch_discover_tests(squishyTest)
