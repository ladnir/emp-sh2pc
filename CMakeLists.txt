cmake_minimum_required (VERSION 2.8.11)
project (emp-sh2pc)
set(NAME "emp-sh2pc")
include_directories(${CMAKE_SOURCE_DIR}/..)
#include_directories(${CMAKE_SOURCE_DIR}/semihonest)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} cmake/ ${CMAKE_SOURCE_DIR}/../emp-tool)
find_package(emp-ot      REQUIRED)
FIND_PACKAGE(emp-tool    REQUIRED)
FIND_PACKAGE(cryptoTools REQUIRED)
FIND_PACKAGE(relic       REQUIRED)
FIND_PACKAGE(gmp         REQUIRED)
INCLUDE_DIRECTORIES(${EMP-OT_INCLUDE_DIRS} ${EMP-TOOL_INCLUDE_DIRS} ${cryptoTools_INCLUDE_DIRS} ${relic_INCLUDE_DIRS} ${GMP_INCLUDE_DIRS})

LINK_DIRECTORIES(${cryptoTools_LINK_DIRS})
message(STATUS "ss ${EMP-OT_INCLUDE_DIRS}")



set(CMAKE_C_FLAGS "-pthread -Wall -march=native -O3 -maes")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg -ggdb")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++14")




set (LIBS "${EMP-TOOL_LIBRARIES};${cryptoTools_LIBRARIES};${relic_LIBRARIES};${GMP_LIBRARIES}")
message(STATUS "LIBS = ${LIBS}")


macro (add_test _name)
	add_executable("test_${_name}" "test/${_name}.cpp" ${basics})
	target_link_libraries("test_${_name}"  ${LIBS}) 
endmacro()

macro (add_bench _name)
	add_executable("bench_${_name}" "bench/${_name}.cpp" ${basics})
	target_link_libraries("bench_${_name}"    ${LIBS}) 
endmacro()

macro (add_example _name)
	add_executable("example_${_name}" "example/${_name}.cpp" ${basics})
	target_link_libraries("example_${_name}"   ${LIBS}) 
endmacro()


set(basics
)

#circuit tests
add_test (bit)
add_test (int)
add_test (float)
add_test (circuit_file)
add_test (all)

add_example (semi_honest)

install(FILES emp-sh2pc-config.cmake DESTINATION cmake/)

file(GLOB  headers 
"semihonest/*.h"
)
install(FILES ${headers}  DESTINATION include/${NAME})
install(FILES emp-sh2pc.h  DESTINATION include)

