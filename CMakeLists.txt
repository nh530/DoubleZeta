# DoubleZeta top most Build and Testing steps.
cmake_minimum_required(VERSION 3.14)
set(CXX clang++) # Specify compiler.
project(DoubleZeta VERSION 1.0 LANGUAGES CXX) # Sets the DoubleZeta_VERSION_MAJOR variable.

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

include(GoogleTest)

########################################  Build step  ############################################################################

add_subdirectory(src) # This target is created by a CMakeList.txt in src directory.
#add_executable(main.o run.cpp)
#target_link_libraries(main.o PUBLIC src)
#target_include_directories(main.o PUBLIC ${PROJECT_SOURCE_DIR}/src)

# End user executables that utilizes our package.
set(EXECUTABLES "${CMAKE_CURRENT_SOURCE_DIR}/examples")
add_executable(zeta.o ${EXECUTABLES}/ZetaSession.test.cpp)
add_executable(main.o ${EXECUTABLES}/main.cpp)
add_executable(optimization.o ${EXECUTABLES}/optimization.cpp)
target_link_libraries(zeta.o PUBLIC src)
target_link_libraries(main.o PUBLIC src)
target_link_libraries(optimization.o PUBLIC src)


################################################ Unit step ######################################################################
# Now simply link against gtest or gtest_main as needed. Eg
add_executable(test_run 
	./tests/temp.test.cpp
	./tests/Linear.test.cpp
	./tests/matrix.test.cpp
	./tests/integration/LinearRegression.test.cpp
)
target_link_libraries(test_run GTest::gtest_main src)

enable_testing()
gtest_discover_tests(test_run)




