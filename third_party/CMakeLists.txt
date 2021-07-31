cmake_minimum_required(VERSION 3.14)
# The FetchModule is only available since CMake 3.11.

include(FetchContent)

FetchContent_GetProperties(ModuleManagerFetch)
if(NOT ModuleManagerFetch_POPULATED)
FetchContent_Declare(
    ModuleManagerFetch
    GIT_REPOSITORY "https://github.com/bradosia/Module-Manager.git"
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/Module-Manager"
)
FetchContent_MakeAvailable(ModuleManagerFetch)
endif()

find_library(LIBRARY_EXISTS ModuleManager)
if(NOT LIBRARY_EXISTS)
add_subdirectory(Module-Manager)
endif()

FetchContent_GetProperties(SettingsManagerFetch)
if(NOT SettingsManagerFetch_POPULATED)
FetchContent_Declare(
    SettingsManagerFetch
    GIT_REPOSITORY "https://github.com/bradosia/Settings-Manager.git"
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/Settings-Manager"
)
FetchContent_MakeAvailable(SettingsManagerFetch)
endif()

find_library(LIBRARY_EXISTS SettingsManager)
if(NOT LIBRARY_EXISTS)
add_subdirectory(Settings-Manager)
endif()
