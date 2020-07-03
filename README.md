# Bookfiler™ Project Manager
A decentralized project management system for real estate management and accounting.

# Features
* Super fast whole-drive file search.
* Robust no-lag handling of displaying directories with millions of files.
* Super fast start-up.
* JSON tree viewer module.
* File OCR module.

# Dependencies
* QT 5.13.2
* boost 1.72.0
* rapidjson v1.1 (2016-8-25)
* SQLite 3.31.1

# Installation

## Mingw64

```shell
pacman -S mingw-w64-x86_64-libgit2
mkdir build
cd build
cmake -G "MSYS Makefiles" ../
```

# Development & Design Architecture

## Module System
![Module System](https://github.com/bradosia/BookFiler-File-Manager/blob/master/UML/module_system_D20200324.png)

Bookfiler™ File Manager has been designed with reusable widgets and unit testing. The modular widgets can be included in other project's personal use or commercial use application.
