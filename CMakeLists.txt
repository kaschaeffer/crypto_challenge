cmake_minimum_required(VERSION 2.8.9)
project (crypto_challenge)

enable_testing()

SET(CMAKE_CXX_FLAGS "-fsanitize=address -fno-omit-frame-pointer -g -std=c++11 -Wall -stdlib=libc++")

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} "src")

file(GLOB SRC "src/*/*.cc")
file(GLOB TEST "test/*/*.cc")

add_executable(runTests ${SRC} ${TEST})
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
add_test(runTests runTests)
