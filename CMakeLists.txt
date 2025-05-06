cmake_minimum_required(VERSION 3.30)
project(computer_club LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(task ${SOURCES})

if(WIN32)
    set_target_properties(task PROPERTIES SUFFIX ".exe")
endif()