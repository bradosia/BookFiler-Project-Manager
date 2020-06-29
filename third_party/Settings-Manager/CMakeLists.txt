cmake_minimum_required(VERSION 3.1)

set(project_name SettingsManager)
project(${project_name} VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(lib_static_name ${project_name})

include_directories(
  third_party/rapidjson/include/
)

set(SOURCES ${SOURCES}
  src/core/SettingsManager.cpp
  src/core/SettingsManager.hpp
)

add_library(${lib_static_name} STATIC ${SOURCES})

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Post Build
# Copy default setttings
add_custom_command(TARGET ${lib_static_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/include/")
add_custom_command(TARGET ${lib_static_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/include/${project_name}/")
add_custom_command(TARGET ${lib_static_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/lib/")
add_custom_command(TARGET ${lib_static_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/src/core/SettingsManager.hpp" "${PROJECT_SOURCE_DIR}/include/${project_name}/")
add_custom_command(TARGET ${lib_static_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${lib_static_name}> "${PROJECT_SOURCE_DIR}/lib/")
