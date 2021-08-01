cmake_minimum_required(VERSION 3.4)
# The FetchModule is only available since CMake 3.11.
# FetchContent_MakeAvailable since CMake 3.14.

include(FetchContent)

FetchContent_Declare(
    ModuleManagerFetch
    GIT_REPOSITORY "https://github.com/bradosia/Module-Manager.git"
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/Module-Manager"
)
FetchContent_MakeAvailable(ModuleManagerFetch)

FetchContent_Declare(
    SettingsManagerFetch
    GIT_REPOSITORY "https://github.com/bradosia/Settings-Manager.git"
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/Settings-Manager"
)
FetchContent_MakeAvailable(SettingsManagerFetch)
