cmake_minimum_required(VERSION 3.26)
set(This BrowserEngineInternals)

project(${This} C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED )

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(BrowserEngineInternals main.cpp)

enable_testing()

add_executable(test_
        Test123.cpp)

target_link_libraries(test_
        GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_)