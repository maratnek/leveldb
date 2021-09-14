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
conan_cmake_run(REQUIRES leveldb/1.22@ benchmark/1.5.6@ protobuf/3.17.1@
        GENERATORS cmake cmake_paths cmake_find_package
        BUILD_TYPE Debug
        BASIC_SETUP CMAKE_TARGETS)

add_executable(leveldb main.cpp)
target_link_libraries(leveldb CONAN_PKG::leveldb)

add_executable(easy-load-leveldb easy-load-leveldb.cpp utils.cpp)
target_link_libraries(easy-load-leveldb CONAN_PKG::leveldb)

add_executable(multi-load-leveldb multi-load-leveldb.cpp utils.cpp)
target_link_libraries(multi-load-leveldb CONAN_PKG::leveldb)

find_package(Protobuf REQUIRED)
set(PROTO_FILES
       proto/first.proto
       proto/block.proto
       )
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})
message(test ${PROTO_SRCS} and ${PROTO_HDRS})
add_executable(proto-real-data proto-real-data.cpp utils.cpp ${PROTO_SRCS} ${PROTO_HRCS})
target_include_directories(proto-real-data PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(proto-real-data CONAN_PKG::leveldb CONAN_PKG::protobuf)

include(CTest)
enable_testing()

add_test(NAME easy-load-leveldb COMMAND easy-load-leveldb)
add_test(NAME multi-load-leveldb COMMAND multi-load-leveldb)
add_test(NAME proto-real-data COMMAND proto-real-data)
