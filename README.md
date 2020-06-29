# Bookfiler™ File Manager
A QT5 pagram for file management.

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

# Development & Design Architecture

## Module System
![Module System](https://github.com/bradosia/BookFiler-File-Manager/blob/master/UML/module_system_D20200324.png)

# Competitors
| Program | License | Description |
|:-- |:-- |:-- |
|qtfm|GPL 2.0|qtfm is also written using QT5 GUI. It is only for linux. It is possible to compile qtfm for windows by replacing posix with windows api.|
|PCManFM|GPL|similar to qtfm|
|Explorer++|GPL 3.0|Uses modern C++. Lot's of windows api usage|
|file-commander|apache-2.0|Uses modern C++14. Threaded. Uses QT5. No platform specific API.|

The advantages of this program is LGPL license and modern C++ usage. file-commander is the most similar to this Bookfiler™ File Manager's goals. The main difference between file-commander and Bookfiler™ File Manager is the UI. file-commander is a dual pane viewer and Bookfiler™ File Manager is a single pane tree view like qtfm.

Bookfiler™ File Manager has been designed with reusable widgets and unit testing. The modular widgets can be included in other project's personal use or commercial use application.
