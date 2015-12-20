cmake_minimum_required(VERSION 2.8.9)
project (crypto_challenge)

enable_testing()

SET(CMAKE_CXX_FLAGS "-fsanitize=address -fno-omit-frame-pointer -g -std=c++11 -Wall -stdlib=libc++")
SET(CMAKE_EXE_LINKER_FLAGS "-lssl -lcrypto")

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} "src")

file(GLOB LIB "src/cryptolib/*.cc")
file(GLOB SOLUTIONS "src/solutions/*.cc")
file(GLOB TEST "test/*/*.cc")

add_executable(runTests ${LIB} ${TEST})
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
add_test(runTests runTests)

add_executable(checkSolutions ${LIB} ${SOLUTIONS})
target_link_libraries(checkSolutions ${GTEST_LIBRARIES} pthread)
add_test(checkSolutions runTests)
