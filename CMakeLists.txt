cmake_minimum_required(VERSION 3.5.1)

# Define project name
project(libCppHelpers VERSION 1.0)

add_compile_options(-O3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_minimum_required(VERSION 3.5.1)

include(GNUInstallDirs)

file(GLOB SOURCES
  "${PROJECT_SOURCE_DIR}/*.cpp"
  "${PROJECT_SOURCE_DIR}/result/*.cpp"
  "${PROJECT_SOURCE_DIR}/string/*.cpp"
  "${PROJECT_SOURCE_DIR}/logging/*.cpp"
  "${PROJECT_SOURCE_DIR}/safe_types/*.cpp"
  "${PROJECT_SOURCE_DIR}/file_system/*.cpp"
)

add_library(${PROJECT_NAME}
  SHARED
    ${SOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME}
 PUBLIC
  result
  string
  logging
  safe_types
  file_system
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
