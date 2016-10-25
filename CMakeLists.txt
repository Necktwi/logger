cmake_minimum_required (VERSION 2.6)
project (logger)
set (logger_VERSION_MAJOR 1)
set (logger_VERSION_MINOR 0)
set (Umbrella "ferryfair")
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/loggerConfig.h.in"
    "${PROJECT_BINARY_DIR}/loggerConfig.h"
    )
configure_file(
    "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.pc.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc"
    )
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

set (GCC_COVERAGE_COMPILE_FLAGS "-std=c++14")
set (GCC_COVERAGE_LINK_FLAGS "-lbase")
include_directories(${CMAKE_CURRENT_SOURCE_DIR} "/usr/local/include/ferryfair")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
file(GLOB SOURCES *.cpp)
add_library(loggerStatic STATIC ${SOURCES})
add_library(logger SHARED ${SOURCES})
set_target_properties(loggerStatic PROPERTIES OUTPUT_NAME logger)
set_target_properties(logger
    PROPERTIES
    VERSION ${logger_VERSION_MAJOR}.${logger_VERSION_MINOR}
    SOVERSION ${logger_VERSION_MAJOR}
    )
set_target_properties(loggerStatic
    PROPERTIES
    VERSION ${logger_VERSION_MAJOR}.${logger_VERSION_MINOR}
    SOVERSION ${logger_VERSION_MAJOR}
    )
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
file(GLOB HEADERS *.h)
install(FILES ${HEADERS} DESTINATION include/${Umbrella})
install(FILES "${PROJECT_BINARY_DIR}/loggerConfig.h"
    DESTINATION include/${Umbrella})
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.pc"
    DESTINATION lib/pkgconfig
    RENAME "lib${PROJECT_NAME}.pc")
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.pc"
    DESTINATION share/pkgconfig
    RENAME "lib${PROJECT_NAME}.pc")
