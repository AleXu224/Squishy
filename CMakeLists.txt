cmake_minimum_required(VERSION 3.22)
set(VCPKG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/glt/vcpkg")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON)
project(squishy)
set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

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

file(GLOB_RECURSE all_files CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")

# add_executable(squishy main.cpp src/Character.hpp src/stat.hpp src/Stats.hpp src/BaseStats.hpp src/Curves.hpp src/Talents.hpp src/StatSheet.hpp src/Weapon.hpp src/WeaponBaseStats.hpp src/WeaponStats.hpp src/Weapons.hpp src/WeaponData.hpp src/Weapons/StaffOfTheScarletSands/StaffOfTheScarletSands.hpp src/CharacterData.hpp src/Characters.hpp src/Characters/Cyno/Cyno.hpp src/Node.hpp src/Element.hpp src/Ui/Library/sqUI.hpp src/Ui/Character/NodeShowcase.hpp src/Ui/Character/SkillShowcase.hpp src/Ui/General/Card.hpp src/Ui/Character/StatShowcase.hpp src/Ui/General/Masonry.hpp src/Ui/General/ElementToColor.hpp src/Ui/Character/CharacterPage.hpp src/Data.hpp src/Artifact.hpp src/ArtifactData.hpp src/Artifacts.hpp src/Artifacts/GildedDreams/GildedDreams.hpp src/Conditional.hpp src/ArtifactIncludes.hpp src/WeaponIncludes.hpp src/CharacterIncludes.hpp src/Ui/Artifact/ArtifactPage.hpp src/Ui/Artifact/ArtifactCard.hpp src/Ui/General/NameValueEntry.hpp src/Ui/General/Grid.hpp src/Rarity.hpp src/Ui/General/RarityToColor.hpp src/Ui/Artifact/ArtifactEdit.hpp)
add_executable(squishy main.cpp ${all_files})
target_include_directories(squishy PRIVATE src)
add_dependencies(squishy Assets)

if (MSVC)
    # add_compile_options(/W4 /WX)
    # add_compile_options(/fsanitize=address)
    target_compile_options(squishy PRIVATE /fsanitize=address)
    target_compile_definitions(squishy PRIVATE _DISABLE_STRING_ANNOTATION _DISABLE_VECTOR_ANNOTATION)
else()
    target_compile_options(squishy PRIVATE -Wall -Wextra -Wno-unused-parameter -Werror -ferror-limit=0)
endif()

find_package(benchmark CONFIG REQUIRED)
target_link_libraries(squishy PRIVATE benchmark::benchmark benchmark::benchmark_main)

add_subdirectory(glt)
include_directories(glt/include)

target_link_libraries(squishy PRIVATE glt)
