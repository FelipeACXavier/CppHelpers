cmake_minimum_required(VERSION 3.5.1)

# Define project name
project(libcpphelpers VERSION 1.0)

add_compile_options(-O3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_minimum_required(VERSION 3.5.1)

include(GNUInstallDirs)

# ------------------------------------------------------------------------------------------------------------
# Options to enable/disable parts
option(CPPHELPERS_SAFE_TYPES   "Do not build safe_types helpers"   ON)
option(CPPHELPERS_FILE_SYSTEM  "Do not build file_system helpers"  ON)

set(LIBCPPHELPERS_SOURCES "")
set(LIBCPPHELPERS_INCLUDES "")

# Results are always built
file(GLOB RESULT_SRC "${PROJECT_SOURCE_DIR}/result/*.cpp")
list(APPEND LIBCPPHELPERS_SOURCES  ${RESULT_SRC})
list(APPEND LIBCPPHELPERS_INCLUDES result)

file(GLOB STRING_SRC "${PROJECT_SOURCE_DIR}/string/*.cpp")
list(APPEND LIBCPPHELPERS_SOURCES  ${STRING_SRC})
list(APPEND LIBCPPHELPERS_INCLUDES string)


file(GLOB LOGGING_SRC "${PROJECT_SOURCE_DIR}/logging/*.cpp")
list(APPEND LIBCPPHELPERS_SOURCES  ${LOGGING_SRC})
list(APPEND LIBCPPHELPERS_INCLUDES logging)

if(CPPHELPERS_SAFE_TYPES)
  file(GLOB SAFE_TYPES_SRC "${PROJECT_SOURCE_DIR}/safe_types/*.cpp")
  list(APPEND LIBCPPHELPERS_SOURCES  ${SAFE_TYPES_SRC})
  list(APPEND LIBCPPHELPERS_INCLUDES safe_types)
endif()

if(CPPHELPERS_FILE_SYSTEM)
  file(GLOB FILE_SYSTEM_SRC "${PROJECT_SOURCE_DIR}/file_system/*.cpp")
  list(APPEND LIBCPPHELPERS_SOURCES  ${FILE_SYSTEM_SRC})
  list(APPEND LIBCPPHELPERS_INCLUDES file_system)
endif()

# ------------------------------------------------------------------------------------------------------------
# Actual compilation
add_library(${PROJECT_NAME} STATIC ${LIBCPPHELPERS_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${LIBCPPHELPERS_INCLUDES}
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
