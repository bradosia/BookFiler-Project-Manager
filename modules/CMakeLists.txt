cmake_minimum_required(VERSION 3.4)
# The FetchModule is only available since CMake 3.11.
# FetchContent_MakeAvailable since CMake 3.14.

# FetchContent vs ExternalProject
# FetchContent populates content from the other project at configure time.
# ExternalProject populates content from the other project at build time. 
include(FetchContent)

FetchContent_Declare(
    BookFilerModuleFilesystemDatabaseFetch
    GIT_REPOSITORY "https://github.com/bradosia/BookFiler-Module-Filesystem-Database.git"
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/modules/BookFiler-Module-Filesystem-Database"
)
FetchContent_MakeAvailable(BookFilerModuleFilesystemDatabaseFetch)

FetchContent_Declare(
    BookFilerModuleFileTreePaneFetch
    GIT_REPOSITORY "https://github.com/bradosia/BookFiler-Module-File-Tree-Pane.git"
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/modules/BookFiler-Module-File-Tree-Pane"
)
FetchContent_MakeAvailable(BookFilerModuleFileTreePaneFetch)
