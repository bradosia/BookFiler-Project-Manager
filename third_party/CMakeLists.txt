cmake_minimum_required(VERSION 3.11)
# The FetchModule is only available since CMake 3.11.

include(FetchContent)

if(NOT ${ModuleManager})
FetchContent_Declare(
    Module-Manager
    GIT_REPOSITORY "https://github.com/bradosia/Module-Manager.git"
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/Module-Manager"
)
FetchContent_MakeAvailable(Module-Manager)
endif()

if(NOT ${SettingsManager})
FetchContent_Populate(
    Settings-Manager
    GIT_REPOSITORY "https://github.com/bradosia/Settings-Manager.git"
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/Settings-Manager"
)
FetchContent_MakeAvailable(Settings-Manager)
endif()
