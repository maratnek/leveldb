cmake_minimum_required(VERSION 3.20)
project(leveldb VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_REQUIRED ON)

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(REQUIRES leveldb/1.22@ benchmark/1.5.6@
        GENERATORS cmake cmake_paths cmake_find_package
        BUILD_TYPE Debug
        BASIC_SETUP CMAKE_TARGETS)

add_executable(leveldb main.cpp)
target_link_libraries(leveldb CONAN_PKG::leveldb)
