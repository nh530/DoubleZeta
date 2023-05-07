# This defines the library build steps for the DoubleZeta source code, which is under the src directory.
project(src)

set(THREADPOOL_DIR "./threadpool")
set(TYPING_DIR "./typing")
set(REGRESSION_DIR "./regression")
set(ALGEBRA_DIR "./algba_cmd")

add_library(
	src 
	SHARED 
	${THREADPOOL_DIR}/temp.tpp
	${TYPING_DIR}/DTypes.h
	${THREADPOOL_DIR}/Job.tpp
	${THREADPOOL_DIR}/ZetaSession.tpp
	${REGRESSION_DIR}/Linear.tpp
	${REGRESSION_DIR}/Regression.cpp
	${ALGEBRA_DIR}/Algebra.tpp
	) # cmake needs the library type to be specified as SHARED in order to build .tpp files.
set_target_properties(src PROPERTIES LINKER_LANGUAGE CXX) # cmake cannot determine the language, so explicitly state cpp.
target_include_directories(src INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}) # CMAKE_CURRENT_SOURCE_DIR is the directory of the CMakeListFile.txt.



