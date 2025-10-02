cmake_minimum_required(VERSION 3.24)

project(theater LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(strong_type CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

add_executable(theater
    src/main.cpp
    src/config/config.cpp
)

target_include_directories(theater PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(theater PRIVATE
    rollbear::strong_type
)

install(TARGETS theater RUNTIME DESTINATION bin)

enable_testing()
add_executable(tests
    tests/main.cpp
    src/config/config.cpp
)
target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(tests PRIVATE
    GTest::gtest_main
    rollbear::strong_type
)
add_test(NAME unit_tests COMMAND tests)

