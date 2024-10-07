# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.21)

# Set the project name and version
project(
    TrueFactionSystem 
    VERSION 0.2.6 
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

# If OUTPUT_FOLDER is set, the compiled plugin will be copied to the appropriate folder
if(DEFINED ENV{SKYRIM_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_FOLDER}/Data")
    set(OUTPUT_FOLDER "$ENV{SKYRIM_FOLDER}/Data")
endif()

if(DEFINED ENV{SKYRIM_MODS_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_MODS_FOLDER}")
    set(OUTPUT_FOLDER "$ENV{SKYRIM_MODS_FOLDER}/${PROJECT_NAME}")
endif()

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/version.rc
        @ONLY)


set(HEADERS
	include/Main.h
    include/Disguise.h
	include/Faction.h
	include/Combat.h
	include/DisguiseData.h
	include/ArmorSlots.h
    include/NpcDetectionData.h
	include/ArmorKeywordData.h
	include/ActorExtensions.h
    include/Race.h
    include/Papyrus.h
)

# Add source files from the src directory
set(SOURCES
    src/Main.cpp
    src/Disguise.cpp
    src/Faction.cpp
    src/Combat.cpp
    src/DisguiseData.cpp
    src/ArmorSlots.cpp
    src/ArmorKeywordData.cpp
    src/ActorExtensions.cpp
    src/Race.cpp
    src/Papyrus.cpp
)

# Find CommonLibSSE NG and setup the SKSE plugin
find_package(CommonLibSSE CONFIG REQUIRED)

# Define the project with multiple source files
add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${HEADERS} ${SOURCES})
add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS "${PROJECT_NAME}")

target_include_directories(${PROJECT_NAME}
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
        $<INSTALL_INTERFACE:src>)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)


target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
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
