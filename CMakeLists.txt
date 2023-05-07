cmake_minimum_required(VERSION 3.14)
project(DoubleZeta VERSION 1.0) # Sets the DoubleZeta_VERSION_MAJOR variable.

# Define variables.
set(CMAKE_CXX_STANDARD 20) # C++20
set(CMAKE_CXX_STANDARD_REQUIRED true) 
set(CMAKE_MODULE_PATH "/Users/normanhong/Documents/coding/c_pp/repos/DoubleZeta/libs")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # For Windows: Prevent overriding the parent project's compiler/linker settings
set(GTEST_URL "https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip")

#configure_file(DoubleZetaConfig.h.in DoubleZetaConfig.h) # it knows to look for input in source dir and output in binary dir.

# For debugging.
message("project binary dir=" ${PROJECT_BINARY_DIR})
message("project source dir=" ${PROJECT_SOURCE_DIR})
message("cmake current source dir =" ${CMAKE_CURRENT_SOURCE_DIR})
message("cmake module path =" ${CMAKE_MODULE_PATH})


########################################## Fetch build dependencies ##############################################################
include(FetchContent)

FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL ${GTEST_URL}
)
FetchContent_MakeAvailable(googletest)


########################################  Build step  ############################################################################

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(test_run ./tests/temp.test.cpp)
target_link_libraries(test_run GTest::gtest_main)

# Directories to include when building the target executable.
#target_include_directories(test_run PUBLIC ${PROJECT_BINARY_DIR})



################################################ Unit step ######################################################################

enable_testing()
include(GoogleTest)
gtest_discover_tests(test_run)




