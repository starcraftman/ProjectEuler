CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ProjectEuler C)

# Variables
SET(DEBUG_FLAGS "-ggdb -Wall -Wextra -Winline -Wfatal-errors -pedantic")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu17 -D_REENTRANT ${DEBUG_FLAGS}")
SET(GTEST_ROOT  "${CMAKE_SOURCE_DIR}/libs")
SET(CLIB "my_c")

FIND_PACKAGE(GTest 1.7 REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

INCLUDE_DIRECTORIES(
    ${GTEST_INCLUDE_DIRS}
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/clib"
)

ADD_SUBDIRECTORY(clib)
ADD_SUBDIRECTORY(src)
#ADD_SUBDIRECTORY(test)

# Debug
#GET_CMAKE_PROPERTY(_variablenames variables)
#FOREACH (_variablename ${_variablenames})
    #MESSAGE(STATUS "${_variableName}=${${_variableName}}")
#ENDFOREACH(_variablename)
