PROJECT(playercore C CXX)
cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Sql REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Sql REQUIRED)
find_library(TAGLIB
    NAMES tag
    PATHS /usr/lib/x86_64-linux-gnu
    DOC "FOUND taglib on this computer")
set(TAGLIB_INCLUDE_DIRS /usr/include/taglib)

INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(${TAGLIB_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Sql_INCLUDE_DIRS})
file(GLOB SOURCES "*.cpp" "include/*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${TAGLIB})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Sql)