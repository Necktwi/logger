# list does not recognize sub-command FILTER for version 3.5
cmake_minimum_required (VERSION 3.5)
include(GNUInstallDirs)
project (logger)
message ("Generating build procedure for ${PROJECT_NAME} ...")

IF (DEFINED _DEBUG)
ADD_DEFINITIONS(-D_DEBUG=${_DEBUG})
ENDIF()
set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)
set (Umbrella "ferryfair")
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
)
configure_file(
  "${PROJECT_SOURCE_DIR}/pkgconfig.pc.in"
  "${PROJECT_BINARY_DIR}/pkgconfig.pc"
)
if (NOT LINKAGE)
  set(LINKAGE "STATIC")
  message("Building static by default")
else ()
  string(TOUPPER "${LINKAGE}" LINKAGE)
endif ()

set (GCC_COVERAGE_COMPILE_FLAGS "-pthread")
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (GCC_COVERAGE_LINK_FLAGS "-fPIC -Wl,-unresolved-symbols=ignore-in-shared-libs")
if (DEFINED _DEBUG)
  set (GCC_COVERAGE_COMPILE_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS} -g -O0")
  set (GCC_COVERAGE_LINK_FLAGS "${GCC_COVERAGE_LINK_FLAGS} -g -O0")
endif()
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
file(GLOB HEADERS *.h)
file(GLOB SOURCES *.cpp)
list(APPEND HEADERS "config.h.in")
include_directories(${PROJECT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} "../"
  ${CMAKE_INSTALL_FULL_INCLUDEDIR}/${Umbrella}
)
message("CMAKE_GENERATOR: ${CMAKE_GENERATOR} CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
link_directories("${CMAKE_INSTALL_FULL_LIBDIR}")
if(CMAKE_CONFIGURATION_TYPES)
  link_directories("../ferrybase/build/$(ConfigurationName)")
else(CMAKE_CONFIGURATION_TYPES)
  link_directories("../ferrybase/build")
endif()
MESSAGE("CMAKE_INSTALL_FULL_LIBDIR ${CMAKE_INSTALL_FULL_LIBDIR}")

if (${LINKAGE} MATCHES "STATIC")
  add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
else ()
  add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
endif()
set_target_properties(${PROJECT_NAME}
  PROPERTIES
  VERSION ${VERSION_MAJOR}.${VERSION_MINOR}
  SOVERSION ${VERSION_MAJOR}
)
target_link_libraries(${PROJECT_NAME} ferrybase)
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${Umbrella})
install(FILES "${PROJECT_BINARY_DIR}/config.h"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${Umbrella}
  RENAME ${PROJECT_NAME}Config.h
)
install(FILES "${PROJECT_BINARY_DIR}/pkgconfig.pc"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
  RENAME "${PROJECT_NAME}.pc"
)
install(FILES "${PROJECT_BINARY_DIR}/pkgconfig.pc"
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
  RENAME "${PROJECT_NAME}.pc"
)
