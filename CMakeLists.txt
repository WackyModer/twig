cmake_minimum_required(VERSION 3.20)
project(twig LANGUAGES CXX)


# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure static linking
set(SDL_STATIC ON CACHE BOOL "Build SDL3 as a static library")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Disable building shared libraries")

# Include FetchContent to download SDL3
include(FetchContent)
message(STATUS "Starting to download SDL3")
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.4  # Use latest stable release
)

# Fetch and build SDL2
FetchContent_MakeAvailable(SDL3)

message(STATUS "Starting to download SDL_ttf")
FetchContent_Declare(
    SDL_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG main  # release 2.24.0
)
FetchContent_MakeAvailable(SDL_ttf)

# Ensure linker can find the static libraries
link_directories(${SDL3_BINARY_DIR}/lib ${SDL_ttf_BINARY_DIR}/lib)

# Add source files
file(GLOB SRC_FILES src/*.cpp)

# Create executable
add_executable(twig ${SRC_FILES})


#set_target_properties(Twig PROPERTIES WIN32 OFF)

# Link SDL3 to the project
target_link_libraries(twig PRIVATE SDL3-static SDL3_ttf-static)

target_include_directories(twig PRIVATE ${SDL3_SOURCE_DIR}/include ${SDL_ttf_SOURCE_DIR}/include)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Allows console.
    # cmake -DCMAKE_BUILD_TYPE=Debug ..
    target_compile_definitions(twig PRIVATE TWIG_DEBUG)
endif()
