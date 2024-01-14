# This defines the library build steps for the DoubleZeta source code, which is under the src directory.
project(src)

set(THREADPOOL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/threadpool")
set(TYPING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/typing")
set(REGRESSION_DIR "${CMAKE_CURRENT_SOURCE_DIR}/regression")
set(ALGEBRA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/algba_cmd")
set(OPTIMIZER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/optimizer")
set(CONFIG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/config")
set(DOUBLE_WORD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/double_word")

set(
	THREADPOOL_FILES 
	${THREADPOOL_DIR}/Job.tpp
	${THREADPOOL_DIR}/ZetaSession.tpp
)
set(
	TYPING_FILES 
	${TYPING_DIR}/DTypes.h
	${TYPING_DIR}/exceptions.cpp
	${TYPING_DIR}/Matrix.tpp
)
set(
	REGRESSION_FILES 
	${REGRESSION_DIR}/Linear.cpp
	${REGRESSION_DIR}/Regression.cpp
)
set(
	ALGEBRA_FILES 
	${ALGEBRA_DIR}/Algebra.tpp
)
set(
	OPTIMIZER_FILES
	${OPTIMIZER_DIR}/GradientDescent.cpp
)
set(
	CONFIG_FILES
	${CONFIG_DIR}/settings.h
)
set(
	DOUBLE_WORD_FILES
	${DOUBLE_WORD_DIR}/bsc_core.h
	${DOUBLE_WORD_DIR}/dw.cpp
	${DOUBLE_WORD_DIR}/dw.h
	${DOUBLE_WORD_DIR}/dw_consts.cpp
	${DOUBLE_WORD_DIR}/fpu_fix.cpp
)



add_library(
	src 
	SHARED 
	${THREADPOOL_FILES}
	${TYPING_FILES}
	${REGRESSION_FILES}
	${ALGEBRA_FILES}
	${OPTIMIZER_FILES}
	${CONFIG_FILES}
	${DOUBLE_WORD_FILES}
	) # cmake needs the library type to be specified as SHARED in order to build .tpp files.
set_target_properties(src PROPERTIES LINKER_LANGUAGE CXX) # cmake cannot determine the language, so explicitly state cpp.
target_include_directories(src INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}) # CMAKE_CURRENT_SOURCE_DIR is the directory of the CMakeListFile.txt.
target_include_directories(src PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}) # This is needed for internal linkage.
target_link_libraries(src GTest::gtest_main)
