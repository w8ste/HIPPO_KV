cmake_minimum_required(VERSION 3.10)

project(hase)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    src/hippo_kv.cpp
    src/main.cpp
)

add_executable(hase ${SOURCES})

# target_link_libraries(hase boost_system boost_filesystem)
