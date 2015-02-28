CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(ProjectEuler C CXX)

# Variables
SET(DEBUG_FLAGS "-ggdb -Wall -Wextra -Winline -Wfatal-errors -pedantic")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D_REENTRANT ${DEBUG_FLAGS}")
SET(BOOST_ROOT  "${CMAKE_SOURCE_DIR}/libs")
SET(GTEST_ROOT  "${BOOST_ROOT}")
SET(CAVE_LIB    "my_cpp")

FIND_PACKAGE(Boost 1.57.0 REQUIRED chrono date_time filesystem regex system thread)
FIND_PACKAGE(GTest 1.7 REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIR}
    ${GTEST_INCLUDE_DIRS}
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/cpplib"
)

ADD_SUBDIRECTORY(src)
#ADD_SUBDIRECTORY(test)
# Debug
#GET_CMAKE_PROPERTY(_variablenames variables)
#FOREACH (_variablename ${_variablenames})
    #MESSAGE(STATUS "${_variableName}=${${_variableName}}")
#ENDFOREACH(_variablename)
