cmake_minimum_required(VERSION 2.8.9)
PROJECT(Playercore CXX)

message("---------------------------------")
message("Configuring ${PROJECT_NAME} ...")
message("---------------------------------")
set(Q_VERSION "5.9.7")
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}-${VERSION_PATCH})
set(TARGET ${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH} /opt/Qt5.9.7/5.9.7/gcc_64/lib")
set(CMAKE_PREFIX_PATH "/opt/Qt5.9.7/5.9.7/gcc_64")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_AUTOMOC ON)
if(NOT INSTALL_PATH)
    set(INSTALL_PATH "$ENV{QML2_IMPORT_PATH}")
    set(INSTALL_PREFIX "${INSTALL_PATH}")
    message("INSTALL PATH :" ${INSTALL_PATH})
    message("INSTALL_PATH variable not set. Using default installation path :" ${INSTALL_PREFIX})
else()
    set(INSTALL_PREFIX ${INSTALL_PATH}/${TARGET})
    message("INSTALL_PATH variable set. Using user  defined installation path :" ${INSTALL_PREFIX})
endif()
set(TARGET_NAME_VERSION "${TARGET}-${VERSION}")
set(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX})
set(INSTALL_LIB_PATH ${INSTALL_PREFIX}/${TARGET}/lib)
set(INSTALL_INCLUDE_PATH ${INSTALL_LIB_PATH}/${TARGET_NAME_VERSION}/include)
set(TARGET_LIB "${INSTALL_LIB_PATH}/${TARGET_NAME_VERSION}/lib${TARGET}.so")
set(FIND_TARGET_MODULE "Find${TARGET}")
message("CMAKE_INSTALL_PREFIX AFTER : " ${CMAKE_INSTALL_PREFIX})
configure_file("config.h.in" "${CMAKE_BINARY_DIR}/config.h")
set(PROJECT_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include")
find_package(Qt5 COMPONENTS Core Sql Quick Multimedia DBus REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Sql REQUIRED)
#find_package(QDBus REQUIRED)
find_library(TAGLIB
    NAMES tag
    PATHS /usr/lib/x86_64-linux-gnu
    DOC "FOUND taglib on this computer")
message("TAGLIB : " ${TAGLIB})
if(TAGLIB)
    message("Found Taglib on this computer")
    set(TAGLIB_INCLUDE_DIRS /usr/include)
endif()


INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(${TAGLIB_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Sql_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${QtDBus_INCLUDE_DIRS})
file(GLOB SOURCES "*.cpp" "include/*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCES})
set_target_properties(${TARGET} PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
    SOVERSION ${VERSION_MAJOR}
)
target_link_libraries(${PROJECT_NAME} ${TAGLIB})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Sql Qt5::Multimedia Qt5::DBus Qt5::Quick)
target_include_directories(${PROJECT_NAME} PUBLIC ${TAGLIB_INCLUDE_DIRS})
install(TARGETS ${TARGET} EXPORT ${TARGET}-targets
    DESTINATION ${INSTALL_LIB_PATH}/${TARGET_NAME_VERSION}
    LIBRARY DESTINATION ${INSTALL_LIB_PATH}/${TARGET_NAME_VERSION}
)

install(EXPORT ${TARGET}-targets DESTINATION ${INSTALL_LIB_PATH}/${TARGET_NAME_VERSION})
export(TARGETS ${TARGET} FILE ${FIND_TARGET_MODULE}.cmake)

configure_file(
   "${PROJECT_SOURCE_DIR}/pkg/playercore-config.cmake.in"
    "${INSTALL_LIB_PATH}/${TARGET_NAME_VERSION}/cmake/${FIND_TARGET_MODULE}.cmake" @ONLY
)

configure_file(
    "${PROJECT_SOURCE_DIR}/pkg/playercore-config-version.cmake.in"
    "${INSTALL_LIB_PATH}/${TARGET_NAME_VERSION}/cmake/${FIND_TARGET_MODULE}-version.cmake" @ONLY
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include
    DESTINATION "${INSTALL_LIB_PATH}/${TARGET_NAME_VERSION}"
    FILES_MATCHING PATTERN "*.h"
)

install(
    FILES ${PROJECT_BINARY_DIR}/config.h
    DESTINATION "${INSTALL_INCLUDE_PATH}"
)
