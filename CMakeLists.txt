# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.21)

# Set the project name and version
project(TrueFactionSystem VERSION 0.0.1 LANGUAGES CXX)

# If OUTPUT_FOLDER is set, the compiled plugin will be copied to the appropriate folder
if(DEFINED ENV{SKYRIM_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_FOLDER}/Data")
    set(OUTPUT_FOLDER "$ENV{SKYRIM_FOLDER}/Data")
endif()

if(DEFINED ENV{SKYRIM_MODS_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_MODS_FOLDER}")
    set(OUTPUT_FOLDER "$ENV{SKYRIM_MODS_FOLDER}/${PROJECT_NAME}")
endif()

# Find CommonLibSSE and setup the SKSE plugin
find_package(CommonLibSSE CONFIG REQUIRED)

# Add source files from the src directory
set(SOURCES
    src/main.cpp
    src/disguise.cpp
    src/faction.cpp
    src/combat.cpp
    src/disguise_data.cpp
    src/armor_slots.cpp
)
include_directories(src)

# Add headers from the include directory
include_directories(include)

# Define the project with multiple source files
add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${SOURCES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_precompile_headers(${PROJECT_NAME} PRIVATE PCH.h)

# Automatically copy the plugin to the mod folder if OUTPUT_FOLDER is set
if(DEFINED OUTPUT_FOLDER)
    set(DLL_FOLDER "${OUTPUT_FOLDER}/SKSE/Plugins")
    message(STATUS "SKSE plugin output folder: ${DLL_FOLDER}")

    add_custom_command(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${DLL_FOLDER}"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        VERBATIM
    )

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(
            TARGET "${PROJECT_NAME}"
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>"
            VERBATIM
        )
    endif()
endif()
