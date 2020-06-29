cmake_minimum_required(VERSION 3.0)

# The plugin name
set(libBasename UCD_OSI_Data)
# The plugin sub directory
set(libIncludeSubdir UCD_OSI_Data)

# Set up source files
set(SOURCES
  Plugin.cpp
  UI/MainWidget.cpp
  UI/TreeModel.cpp
  UI/TreeItem.cpp
  core/HTTP_JSON.cpp
  core/PiWebAPI.cpp
)

set(HEADERS
  Plugin.hpp
  Interface.hpp
  UI/MainWidget.hpp
  UI/universalInclude.hpp
  UI/TreeModel.hpp
  UI/TreeItem.hpp
  core/config.hpp
  core/HTTP_JSON.hpp
  core/PiWebAPI.hpp
)

set(SHARED_COMPILE_DEFINITIONS
  -DSTATIC_LIBRARY
)

set(STATIC_COMPILE_DEFINITIONS
  -DSTATIC_LIBRARY
)

set(SHARED_LINK_LIBRARIES
  Qt5::Widgets

  # curl
  curl
  nghttp2
  psl.dll
  idn2
  gnutls
  ssl
  crypto
  cares
  ssh2
  brotlidec-static
  brotlienc-static
  brotlicommon-static
  unistring
  wldap32
  libz
  bcrypt
  crypt32

  # static link a bunch of stuff
  z
  windowscodecs
  stdc++
  gcc
  opengl32
  gdi32
  comdlg32
  oleaut32
  imm32
  winmm
  winspool
  kernel32
  user32
  shell32
  uuid
  ole32
  advapi32
  ws2_32
  mingw32
  rpcrt4
  tiff
  # std::filesystem
  stdc++fs
)

set(STATIC_LINK_LIBRARIES
  Qt5::Widgets

  # curl
  curl
  nghttp2
  psl.dll
  idn2
  gnutls
  ssl
  crypto
  cares
  ssh2
  brotlidec-static
  brotlienc-static
  brotlicommon-static
  unistring
  wldap32
  libz
  bcrypt
  crypt32

  # static link a bunch of stuff
  z
  windowscodecs
  stdc++
  gcc
  opengl32
  gdi32
  comdlg32
  oleaut32
  imm32
  winmm
  winspool
  kernel32
  user32
  shell32
  uuid
  ole32
  advapi32
  ws2_32
  mingw32
  rpcrt4
  tiff
  # std::filesystem
  stdc++fs
)

set(CMAKE_CXX_STANDARD 17)

# Set up library names
if(BUILD_SHARED_LIBS)
  set(libsh_name ${libBasename}SharedLib)
  set(libsh_name_decorated ${libsh_name}${JKQtPlotter_LIBNAME_ADDITION})
  set(${libBasename}_SHARED_LIBRARY ${libsh_name_decorated}  CACHE STRING "name of static library for ${libBasename}" FORCE)
  message( STATUS "-- Building ${libsh_name}" )
endif(BUILD_SHARED_LIBS)

if(BUILD_STATIC_LIBS)
  set(lib_name ${libBasename}Lib)
  set(lib_name_decorated ${lib_name}${JKQtPlotter_LIBNAME_ADDITION})
  set(${libBasename}_STATIC_LIBRARY ${lib_name_decorated}  CACHE STRING "name of static library for ${libBasename}" FORCE)
  message( STATUS "-- Building ${lib_name}" )
endif(BUILD_STATIC_LIBS)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include(CMakePackageConfigHelpers)

if(BUILD_SHARED_LIBS)
  add_library(${libsh_name} SHARED  ${SOURCES} ${HEADERS})
  target_compile_features(${libsh_name} PUBLIC cxx_std_17)
  set_property(TARGET ${libsh_name} PROPERTY VERSION "${PROJECT_VERSION}")
  set_property(TARGET ${libsh_name} PROPERTY OUTPUT_NAME  "${libsh_name_decorated}")
  target_include_directories(${libsh_name} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )
  set_target_properties(${libsh_name} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS "ON")
  write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${libsh_name}Version.cmake
    VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion )

  # Compile Definitions
  target_compile_definitions(${libsh_name} PUBLIC ${SHARED_COMPILE_DEFINITIONS})
  # Libraries to link
  target_link_libraries(${libsh_name} PUBLIC ${SHARED_LINK_LIBRARIES})
endif()

if(BUILD_STATIC_LIBS)
  add_library(${lib_name} STATIC  ${SOURCES} ${HEADERS})
  target_compile_features(${lib_name} PUBLIC cxx_std_17)
  set_property(TARGET ${lib_name} PROPERTY VERSION "${PROJECT_VERSION}")
  set_property(TARGET ${lib_name} PROPERTY OUTPUT_NAME  "${lib_name_decorated}")
  target_include_directories(${lib_name} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )
  write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Version.cmake
    VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion )

  # Compile Definitions
  target_compile_definitions(${lib_name} PUBLIC ${STATIC_COMPILE_DEFINITIONS})
  # Libraries to link
  target_link_libraries(${lib_name} PUBLIC ${STATIC_LINK_LIBRARIES})
endif()

# Post build
if(BUILD_SHARED_LIBS)
    add_custom_command(TARGET ${libsh_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/plugins/")
    add_custom_command(TARGET ${libsh_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${libsh_name}> "${CMAKE_BINARY_DIR}/plugins/")
endif(BUILD_SHARED_LIBS)

# Installation
if(BUILD_SHARED_LIBS)
  message(STATUS "INSTALL: ${libsh_name} TO root=${CMAKE_BINARY_DIR} and current=${CMAKE_CURRENT_BINARY_DIR}")
  install(TARGETS ${libsh_name}
    RUNTIME DESTINATION ${CMAKE_BINARY_DIR}
    ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}
    LIBRARY DESTINATION ${CMAKE_BINARY_DIR}
  )
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${libsh_name}Version.cmake"
  DESTINATION lib/cmake/${libsh_name}  )
endif(BUILD_SHARED_LIBS)

if(BUILD_STATIC_LIBS)
  install(TARGETS ${lib_name} EXPORT ${lib_name}_TARGETS
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
  install(EXPORT ${lib_name}_TARGETS
    FILE ${lib_name}Config.cmake
    DESTINATION lib/cmake/${lib_name}
  )

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Version.cmake"
  DESTINATION lib/cmake/${lib_name}  )
endif(BUILD_STATIC_LIBS)

install(FILES ${HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}
COMPONENT Headers)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${lib_name}_Readme.txt" DESTINATION doc/ )
install(FILES "${CMAKE_SOURCE_DIR}/LICENSE" DESTINATION doc/ RENAME "${lib_name}_LICENSE.txt" )
