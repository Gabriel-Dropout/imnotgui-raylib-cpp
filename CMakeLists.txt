cmake_minimum_required(VERSION 3.14)
project(
  imnotgui
  VERSION 0.1
  DESCRIPTION "imnotgui library and examples"
  LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)

# Fetch Raylib
find_package(raylib 4.5.0 QUIET)
include(FetchContent)
if (NOT raylib_FOUND)
    #FetchContent will download and extract this file
    FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/4.5.0.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

        option(CUSTOMIZE_BUILD "" ON)
        option(SUPPORT_SCREEN_CAPTURE "" OFF)

        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

# Fetch Raylib-cpp
FetchContent_Declare(
    raylib_cpp
    URL https://github.com/RobLoach/raylib-cpp/archive/refs/tags/v4.5.0.tar.gz
)
FetchContent_MakeAvailable(raylib_cpp)

add_subdirectory(src)
add_subdirectory(example)

#Important: target_link_libraries also propagates to headers. You can "link" header only libraries in CMake
