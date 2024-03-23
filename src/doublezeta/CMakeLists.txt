# This defines the library build steps for the DoubleZeta source code, which is under the src directory.
set(THREADPOOL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/threadpool")
set(TYPING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/typing")
set(REGRESSION_DIR "${CMAKE_CURRENT_SOURCE_DIR}/regression")
set(OPTIMIZER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/optimizer")
set(CONFIG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/config")
set(DOUBLE_WORD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/double_word")

set(
	THREADPOOL_FILES 
	${THREADPOOL_DIR}/Job.tpp
	${THREADPOOL_DIR}/ZetaSession.tpp
	${THREADPOOL_DIR}/Algebra.tpp
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
set(
	CPYTHON_WRAPPERS
	${CMAKE_CURRENT_SOURCE_DIR}/matrix_cpy.cpp
)


add_library(
	src 
	SHARED 
	${THREADPOOL_FILES}
	${TYPING_FILES}
	${REGRESSION_FILES}
	${OPTIMIZER_FILES}
	${CONFIG_FILES}
	${DOUBLE_WORD_FILES}
	) # cmake needs the library type to be specified as SHARED in order to build .tpp files.
#python_extension_module(src) # from PythonExtensions
set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set_target_properties(src PROPERTIES OUTPUT_NAME "core")
set_target_properties(src PROPERTIES LINKER_LANGUAGE CXX) # cmake cannot determine the language, so explicitly state cpp.
target_include_directories(src INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}) # CMAKE_CURRENT_SOURCE_DIR is the directory of the CMakeListFile.txt.
target_include_directories(src PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}) # This is needed for internal linkage.
if (SKBUILD)
	add_library(wrappers MODULE ${CPYTHON_WRAPPERS}) 
	python_extension_module(wrappers) # from PythonExtensions
	set_target_properties(wrappers PROPERTIES OUTPUT_NAME "zetacore")
endif()
