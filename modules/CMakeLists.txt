cmake_minimum_required(VERSION 3.14)
# The FetchModule is only available since CMake 3.14.

# FetchContent vs ExternalProject
# FetchContent populates content from the other project at configure time.
# ExternalProject populates content from the other project at build time. 
include(FetchContent)

find_library(LIBRARY_EXISTS BookFilerModuleFilesystemDatabase-LibShared)
if(NOT LIBRARY_EXISTS)
FetchContent_Declare(
    BookFiler-Module-Filesystem-Database
    GIT_REPOSITORY "https://github.com/bradosia/BookFiler-Module-Filesystem-Database.git"
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/modules/BookFiler-Module-Filesystem-Database"
)
FetchContent_Populate(BookFiler-Module-Filesystem-Database)
add_subdirectory(BookFiler-Module-Filesystem-Database)
endif()

find_library(LIBRARY_EXISTS BookFilerModuleFileTreePane-LibShared)
if(NOT LIBRARY_EXISTS)
FetchContent_Declare(
    BookFiler-Module-File-Tree-Pane
    GIT_REPOSITORY "https://github.com/bradosia/BookFiler-Module-File-Tree-Pane.git"
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/modules/BookFiler-Module-File-Tree-Pane"
)
FetchContent_Populate(BookFiler-Module-File-Tree-Pane)
add_subdirectory(BookFiler-Module-File-Tree-Pane)
endif()
