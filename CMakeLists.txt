cmake_minimum_required(VERSION 3.24)
project(DoubleZeta)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(../build/executeTests ZetaSession.test.cpp)
target_link_libraries(../build/executeTests ${GTEST_LIBRARIES} pthread)
