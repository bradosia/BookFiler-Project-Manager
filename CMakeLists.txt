cmake_minimum_required(VERSION 3.11)
# The FetchModule is only available since CMake 3.11.

set(PROJECT_NAME BookFiler-Project-Manager)
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)
set(RELEASE_DIR "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RELEASE_DIR})

# Configurable Options
OPTION(BUILD_SHARED_LIBS "Build shared libraries" OFF)
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
  third_party/rapidjson/include/
  third_party/Module-Manager/include/
  third_party/Settings-Manager/include/
  src/modules
)

set(LINK_DIRECTORIES
   lib/
   third_party/Module-Manager/lib/
   third_party/Settings-Manager/lib/
)

# MODULES
add_subdirectory(src/modules/)
add_subdirectory(third_party)

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

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES})

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

set(SHARED_LINK_LIBRARIES ${LIBRARIES})
set(STATIC_LINK_LIBRARIES ${LIBRARIES})

link_directories(${LINK_DIRECTORIES})

# Set up library names
if(BUILD_SHARED_LIBS)
  set(lib_shared_name ${PROJECT_NAME}Shared)
  set(lib_shared_name_decorated ${lib_shared_name}${LIBNAME_ADDITION})
  set(${PROJECT_NAME}_SHARED_LIBRARY ${lib_shared_name_decorated} CACHE STRING "name of static library for ${PROJECT_NAME}" FORCE)
  message( STATUS "-- Building ${lib_shared_name}" )
endif(BUILD_SHARED_LIBS)

if(BUILD_STATIC_LIBS)
  set(lib_static_name ${PROJECT_NAME})
  set(lib_static_name_decorated ${lib_static_name}${LIBNAME_ADDITION})
  set(${PROJECT_NAME}_STATIC_LIBRARY ${lib_static_name_decorated} CACHE STRING "name of static library for ${PROJECT_NAME}" FORCE)
  message( STATUS "-- Building ${lib_static_name}" )
endif(BUILD_STATIC_LIBS)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(BUILD_SHARED_LIBS)
  add_library(${lib_shared_name} SHARED  ${SOURCES} ${HEADERS})
  target_compile_features(${lib_shared_name} PUBLIC cxx_std_17)
  set_property(TARGET ${lib_shared_name} PROPERTY VERSION "${PROJECT_VERSION}")
  set_property(TARGET ${lib_shared_name} PROPERTY OUTPUT_NAME  "${lib_shared_name_decorated}")
  target_include_directories(${lib_shared_name} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    ${INCLUDE_DIRECTORIES}
  )
  set_target_properties(${lib_shared_name} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS "ON")
  if(UNIX)
      #set_property(TARGET ${lib_shared_name} PROPERTY POSITION_INDEPENDENT_CODE ON)
  endif()
  write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${lib_shared_name}Version.cmake
    VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion )

  # Compile Definitions
  target_compile_definitions(${lib_shared_name} PUBLIC ${SHARED_COMPILE_DEFINITIONS})
  # Libraries to link
  target_link_libraries(${lib_shared_name} PUBLIC ${SHARED_LINK_LIBRARIES})
endif()

if(BUILD_STATIC_LIBS)
  add_library(${lib_static_name} STATIC  ${SOURCES} ${HEADERS})
  target_compile_features(${lib_static_name} PUBLIC cxx_std_17)
  set_property(TARGET ${lib_static_name} PROPERTY VERSION "${PROJECT_VERSION}")
  set_property(TARGET ${lib_static_name} PROPERTY OUTPUT_NAME  "${lib_static_name_decorated}")
  target_include_directories(${lib_static_name} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    ${INCLUDE_DIRECTORIES}
  )
  write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${lib_static_name}Version.cmake
    VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion )

  # Compile Definitions
  target_compile_definitions(${lib_static_name} PUBLIC ${STATIC_COMPILE_DEFINITIONS})
  # Libraries to link
  target_link_libraries(${lib_static_name} PUBLIC ${STATIC_LINK_LIBRARIES})
endif()

if(BUILD_SHARED_LIBS)
    SET(COMMAND_TARGET ${lib_shared_name})
elseif(BUILD_STATIC_LIBS)
    SET(COMMAND_TARGET ${lib_static_name})
endif()

# Pre build
SET(COPY_HEADERS ${HEADERS})
list(TRANSFORM COPY_HEADERS PREPEND "${PROJECT_SOURCE_DIR}/")
add_custom_command(TARGET ${COMMAND_TARGET} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/include/"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}"
    COMMAND ${CMAKE_COMMAND} -E copy ${COPY_HEADERS} "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}"
)

# Post build
add_custom_command(TARGET ${COMMAND_TARGET} POST_BUILD
    # Make a release directory without all the cmake extras.
    # This directory can easily be archived and distributed.
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RELEASE_DIR}"
    # Create release sub-folders
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RELEASE_DIR}/modules/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/resources/" "${RELEASE_DIR}/share"
)

# Installation
if(BUILD_SHARED_LIBS)
  message(STATUS "INSTALL: ${lib_shared_name} TO root=${CMAKE_BINARY_DIR} and current=${CMAKE_CURRENT_BINARY_DIR}")
  install(TARGETS ${lib_shared_name}
    RUNTIME DESTINATION "${RELEASE_DIR}/modules"
    ARCHIVE DESTINATION "${RELEASE_DIR}/modules"
    LIBRARY DESTINATION "${RELEASE_DIR}/modules"
  )
endif()

if(BUILD_STATIC_LIBS)
  install(TARGETS ${lib_static_name} EXPORT ${lib_static_name}_TARGETS
    RUNTIME DESTINATION "${RELEASE_DIR}/lib"
    ARCHIVE DESTINATION "${RELEASE_DIR}/lib"
    LIBRARY DESTINATION "${RELEASE_DIR}/lib"
  )
endif()

install(FILES ${HEADERS}
  DESTINATION "${RELEASE_DIR}/headers"
COMPONENT Headers)

install(FILES "${CMAKE_SOURCE_DIR}/LICENSE" DESTINATION "${RELEASE_DIR}" RENAME "LICENSE.txt" )
