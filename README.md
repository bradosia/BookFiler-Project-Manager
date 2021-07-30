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
pacman -S mingw-w64-x86_64-libgit2 mingw-w64-x86_64-qt5
mkdir build
cd build
cmake -G "MSYS Makefiles" ../
```

# Development & Design Architecture

## Module System
![Module System](https://github.com/bradosia/BookFiler-File-Manager/blob/master/UML/module_system_D20200324.png)

Bookfiler™ File Manager has been designed with reusable widgets and unit testing. The modular widgets can be included in other project's personal use or commercial use application depending on the module licensing. Modules are implemented using `boost::dll` as the module loader. QT also includes a plugin manager, but boost::dll is used instead to reduce dependency on QT in case another User Interface library is used or considered.

The advantage of runtime loaded DLLs is that the module dependencies do not need to be linked to the main program. The disadvantage is that if modules use the same dependencies then the linker can't optimize the file size.
