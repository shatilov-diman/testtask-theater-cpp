cmake_minimum_required(VERSION 3.24)

project(theater LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED COMPONENTS program_options)
find_package(strong_type CONFIG REQUIRED)

add_executable(theater
    src/main.cpp
)

target_include_directories(theater PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(theater PRIVATE
    Boost::program_options
    rollbear::strong_type
)

install(TARGETS theater RUNTIME DESTINATION bin)

