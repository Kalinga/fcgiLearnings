cmake_minimum_required(VERSION 2.6)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(../cereal/include)

SET(CPP11_COMPILE_FLAGS "-std=c++11")
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CPP11_COMPILE_FLAGS}" )

# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests TestAll.cpp NetworkDataTest.cpp)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)