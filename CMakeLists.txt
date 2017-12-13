cmake_minimum_required (VERSION 3.5)
include(GNUInstallDirs)
project (logger)
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

set (GCC_COVERAGE_COMPILE_FLAGS "-std=c++14")
set (GCC_COVERAGE_LINK_FLAGS "-fPIC -Wl,-unresolved-symbols=ignore-in-shared-libs")
IF (DEFINED _DEBUG)
set (GCC_COVERAGE_COMPILE_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS} -g -O0")
set (GCC_COVERAGE_LINK_FLAGS "${GCC_COVERAGE_LINK_FLAGS} -g -O0")
ENDIF()
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
file(GLOB HEADERS *.h)
file(GLOB SOURCES *.cpp)
include_directories(${PROJECT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} "../" ${CMAKE_INSTALL_FULL_INCLUDEDIR}/${Umbrella})
link_directories("${CMAKE_INSTALL_FULL_LIBDIR}" "../ferrybase/build" "../ferrybase/buildWin64/Debug")
MESSAGE("CMAKE_INSTALL_FULL_LIBDIR ${CMAKE_INSTALL_FULL_LIBDIR}")
add_library(${PROJECT_NAME}Static STATIC ${SOURCES} ${HEADERS})
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
set_target_properties(${PROJECT_NAME}Static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}
    SOVERSION ${VERSION_MAJOR}
)
set_target_properties(${PROJECT_NAME}Static
    PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}
    SOVERSION ${VERSION_MAJOR}
)
target_link_libraries(${PROJECT_NAME}Static ferrybase)
target_link_libraries(${PROJECT_NAME} ferrybase)
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS ${PROJECT_NAME}Static DESTINATION ${CMAKE_INSTALL_LIBDIR})
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
