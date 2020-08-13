cmake_minimum_required(VERSION 2.8.12)

project(ogl CXX)

set( BUILD_SHARED_LIBS OFF )
set( CMAKE_MACOSX_RPATH ON )

# enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if( CMAKE_COMPILER_IS_GNUCXX )
  set( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories( 
    ext/
    ext/glfw/include
    ext/assimp/include
    ext/glm/
    ext/bullet3/src
)

add_subdirectory(ext)

add_subdirectory(src)

