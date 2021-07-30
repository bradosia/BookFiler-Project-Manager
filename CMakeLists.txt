cmake_minimum_required(VERSION 3.11)
# The FetchModule is only available since CMake 3.11.

set(PROJECT_NAME BookFiler-Project-Manager)
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)
set(RELEASE_DIR "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RELEASE_DIR})

# Configurable Options
OPTION(BUILD_SHARED_LIBS "Build shared libraries" ON)
OPTION(BUILD_STATIC_LIBS "Build static libraries" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10")

find_package(Qt5Widgets REQUIRED)
find_package(Boost 1.56 REQUIRED COMPONENTS
             system filesystem)

# TODO: Use Verdigris so no MOC needed
# TODO: Explicitly call RCC tool in prebuild command
# QT tools
# Automatically compile .ui to .h
# set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(INCLUDE_DIRECTORIES
  include/
  third_party/rapidjson/include/
  third_party/Module-Manager/include/
  third_party/Settings-Manager/include/
  modules/
)

set(LINK_DIRECTORIES
   lib/
   third_party/Module-Manager/lib/
   third_party/Settings-Manager/lib/
)

# MODULES
add_subdirectory(third_party)
if(NOT ${ModuleManager})
    add_subdirectory(third_party/Module-Manager)
endif()
if(NOT ${SettingsManager})
    add_subdirectory(third_party/Settings-Manager)
endif()
add_subdirectory(modules/)
add_subdirectory(modules/BookFiler-Module-File-Tree-Pane)
add_subdirectory(modules/BookFiler-Module-Filesystem-Database)

set(SOURCES ${SOURCES}
  src/UI/main.cpp
  src/UI/Application.cpp
  src/UI/MainWindow.cpp
  src/UI/RenderWidget.cpp
)

set(HEADERS ${HEADERS}
  src/UI/Application.hpp
  src/UI/MainWindow.hpp
  src/UI/RenderWidget.hpp
  src/UI/BookfilerRenderWidget.hpp
  src/UI/ui_main.hpp
  src/core/config.hpp

  include/BookFiler-Module-Filesystem-Database/Interface.hpp
  include/BookFiler-Module-File-Tree-Pane/Interface.hpp
)

set(RESOURCES ${RESOURCES}
  resources/icons_file.qrc
)

set(LINK_DIRECTORIES ${LINK_DIRECTORIES}
  lib/
)

set(LIBRARIES ${LIBRARIES}
  # QT5
  Qt5::Widgets

  # Boost
  Boost::system
  Boost::filesystem

  # bradosia
  ModuleManager
  SettingsManager
)

if(WIN32)
    set(LIBRARIES ${LIBRARIES}
        # Windows Libraries
    )
elseif(UNIX)
    set(LIBRARIES ${LIBRARIES}
        # Unix Libraries
    )
endif()

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES})
include_directories(${INCLUDE_DIRECTORIES})
link_directories(${LINK_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARIES})

# Pre build
SET(COPY_HEADERS ${HEADERS})
list(TRANSFORM COPY_HEADERS PREPEND "${PROJECT_SOURCE_DIR}/")
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/third_party/Module-Manager"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/third_party/Settings-Manager"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/modules/BookFiler-Module-File-Tree-Pane"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/modules/BookFiler-Module-Filesystem-Database"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/include/"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}"
    COMMAND ${CMAKE_COMMAND} -E copy ${COPY_HEADERS} "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/lib/"
)

# Post build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    # Make a release directory without all the cmake extras.
    # This directory can easily be archived and distributed.
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RELEASE_DIR}"
    # Create release sub-folders
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RELEASE_DIR}/modules/"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RELEASE_DIR}/resources/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/resources/" "${RELEASE_DIR}/resources"
)

# Installation
install(FILES ${HEADERS}
  DESTINATION "${CMAKE_BINARY_DIR}/include/${PROJECT_NAME}/"
COMPONENT Headers)

install(FILES "${CMAKE_SOURCE_DIR}/LICENSE" DESTINATION "${RELEASE_DIR}" RENAME "LICENSE.txt" )
