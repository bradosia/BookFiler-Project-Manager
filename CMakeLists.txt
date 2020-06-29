cmake_minimum_required(VERSION 3.1)

set(application_name BookFiler-Recognize)
project(${application_name} VERSION 1.0.0 LANGUAGES CXX)
set(RELEASE_DIR "${CMAKE_BINARY_DIR}/${application_name}/")
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
# set(CMAKE_INCLUDE_CURRENT_DIR ON)
# set(QTDIR $ENV{QTDIR})
# set(CMAKE_PREFIX_PATH ${QTDIR})
# set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

include_directories(
  third_party/rapidjson/include/
  third_party/Module-Manager/include/
  third_party/Settings-Manager/include/
  src/modules
)

link_directories(
   lib/
   third_party/Module-Manager/lib/
   third_party/Settings-Manager/lib/
)

# MODULES
add_subdirectory(src/modules/)
add_subdirectory(third_party/Module-Manager/)
add_subdirectory(third_party/Settings-Manager/)

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

  src/interfaces/
)

set(RESOURCES ${RESOURCES}
  resources/icons_file.qrc
)

add_executable(${application_name} ${SOURCES} ${HEADERS} ${RESOURCES})

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
        z
        stdc++
        gcc
        uuid

        gdi32
        comdlg32
        oleaut32
        imm32
        winmm
        winspool
        kernel32
        user32
        shell32
        ole32
        advapi32
        ws2_32
        mingw32
        rpcrt4
    )
elseif(UNIX)
    set(LIBRARIES ${LIBRARIES}
        # Unix Libraries
        z
        stdc++
        gcc
        uuid

        pthread
        glib-2.0
        gthread-2.0
        dl
        m
    )
endif()

target_link_libraries(${application_name} PRIVATE ${LIBRARIES})

# OS-Architecture specific libraries
if(APPLE)
  find_library(COCOA_FRAMEWORK Cocoa)
  target_link_libraries(${application_name} ${COCOA_FRAMEWORK})
endif()

# OS-Architecture specific properties
if(WIN32)
  set_target_properties(${application_name} PROPERTIES
    WIN32 TRUE
  )
elseif(APPLE)
  set_target_properties(${application_name} PROPERTIES
    MACOSX_BUNDLE TRUE
    RESOURCE "${RESOURCES}"
  )
elseif(UNIX)
  #set_property(TARGET ${application_name} PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

# Pre Build
# Make a release directory without all the cmake extras.
# This directory can easily be archived and distributed.
add_custom_command(TARGET ${application_name} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/${application_name}/")
# Create release sub-folders
add_custom_command(TARGET ${application_name} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RELEASE_DIR}/traineddata/"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RELEASE_DIR}/fonts/"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RELEASE_DIR}/modules/"
)
# Post Build
# Copy default setttings
add_custom_command(TARGET ${application_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/resources/settings.json" "${RELEASE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/resources/traineddata/" "${RELEASE_DIR}/traineddata/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/resources/fonts/" "${RELEASE_DIR}/fonts/"
)
add_custom_command(TARGET ${application_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:Qt5::Core>" "${RELEASE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:Qt5::Gui>" "${RELEASE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:Qt5::Widgets>" "${RELEASE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5Core_DIR}/../../../bin/Qt5OpenGL.dll "${RELEASE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RELEASE_DIR}/platforms/"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RELEASE_DIR}/styles/"
)
# OS-Architecture specific
# https://doc.qt.io/qt-5/qpa.html
if(WIN32)
    # Windows support
    add_custom_command(TARGET ${application_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5Core_DIR}/../../../share/qt5/plugins/platforms/qwindows.dll "${RELEASE_DIR}/platforms/"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5Core_DIR}/../../../share/qt5/plugins/styles/qwindowsvistastyle.dll "${RELEASE_DIR}/styles/"
    )
elseif(APPLE)
    # Cocoa support
    add_custom_command(TARGET ${application_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5Core_DIR}/../../../share/qt5/plugins/platforms/libqcocoa.dylib "${RELEASE_DIR}/platforms/"
    )
elseif(UNIX)
    # X Window System (X11) support
    add_custom_command(TARGET ${application_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5Core_DIR}/../../../share/qt5/plugins/platforms/qxcb.so "${RELEASE_DIR}/platforms/"
    )
endif()
