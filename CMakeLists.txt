cmake_minimum_required(VERSION 2.8)

project(Arkneo)

set(ARKNEO_SOURCES_PATH ${CMAKE_CURRENT_LIST_DIR}/src)
set(ARKNEO_INCLUDES_PATH ${CMAKE_CURRENT_LIST_DIR}/include)
set(ARKNEO_CMAKE_FOLDER_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)

# Configuration variables
set(ENABLE_SOURCE_GROUPING true CACHE BOOL "Enable source subfolders")
set(GLM_ROOT "${CMAKE_CURRENT_LIST_DIR}/Dependencies/glm" CACHE PATH "Path to GLM root directory")

# Source File (*.cpp)
set(ARKNEO_SOURCE_FILES
    STL.cpp
    Serialization.cpp
    main.cpp
)

# Headers File (*.hh, *.hpp)
set(ARKNEO_HEADER_FILES
    Sery_Amalgamated.hh
    STL.hh
    Serialization.hh
    Triangle.hh
)

set(CMAKE_MODULE_PATH ${ARKNEO_CMAKE_FOLDER_PATH}/Modules ${CMAKE_MODULE_PATH})
include(${ARKNEO_CMAKE_FOLDER_PATH}/Source_Grouping.cmake)

# Source grouping setup
if (ENABLE_SOURCE_GROUPING)
    SOURCE_GROUP(Headers REGULAR_EXPRESSION ".*\\.h(h|pp)")
    SOURCE_GROUP(Sources REGULAR_EXPRESSION ".*\\.cpp")

    GroupSources(ARKNEO_HEADER_FILES ${ARKNEO_INCLUDES_PATH} Headers)
    GroupSources(ARKNEO_SOURCE_FILES ${ARKNEO_SOURCES_PATH} Sources)
endif()

prefix_custom_list(ARKNEO_HEADER_PATHS "${ARKNEO_INCLUDES_PATH}/" ARKNEO_HEADER_FILES)
prefix_custom_list(ARKNEO_SOURCE_PATHS "${ARKNEO_SOURCES_PATH}/" ARKNEO_SOURCE_FILES)

# Build the binary.
add_executable(Arkneo ${ARKNEO_SOURCE_PATHS} ${ARKNEO_HEADER_PATHS})
include_directories(Arkneo ${ARKNEO_INCLUDES_PATH})

# Find GLM
find_package(GLM REQUIRED)
include_directories(Arkneo ${GLM_INCLUDE_DIR})

if (UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -W -Wall -Wextra -O3")
endif (UNIX)

# Copies the config and media files
add_custom_command(TARGET Arkneo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_LIST_DIR}/Files"
        $<TARGET_FILE_DIR:Arkneo>/Files)

if (WIN32)
# Copies again the files for Windows
    add_custom_command(TARGET Arkneo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_LIST_DIR}/Files"
            ${CMAKE_BINARY_DIR}/Files)
endif(WIN32)