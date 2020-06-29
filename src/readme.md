# Source Code Organization
* /UI/ directory contains anything that mentions or uses any part of QT or User Interface libraries.
* /core/ directory includes non-QT logic
* /plugins/ contain different units of the application

The general idea is that presentation should be separated from logic, but QT empasses many feature beyond the UI. In case QT falls out of popularity in the future, all source files containing an QT Objects will be placed in /UI/ even if it is logic using a QT library.

The use of the word plugins is misleading because they are not meant to extend the core functionality, but instead the application is attempted to be split up into units called plugins so they may be reused and tested individually. The directory is also used for plugins that extend the core too. Possibly in the future there will be some diferentiation.

# Plugins
Plugins are compiled as shared libraries and are loaded using an interface specifying header and boost::dll as the shared library loader. QT also includes a plugin manager, but boost::dll is used instead to reduce dependency on boost in case another User Interface library is used or considered.

# Dependencies

These are the features and the library that is used to implement them. QT probably contains most of these libraries, but use of QT for anything other than UI will be discouraged.

| Feature       | QT           | Other  |
| ------------- | ------------- | ----- |
| User Interface      | User Interface Compiler (UIC) |  |
| Slots & Signals      | Meta-Object Compiler (MOC) |  |
| Embedded Resources | Resource Compiler (RCC) | |
| Plugins | | boost::dll |
| Networking | | curl OR boost::beast |
| SQL Database Connector | | MariaDB Connector (Works with mySQL) |
| JSON | | RapidJSON |
| Configuration Files | | Hjson |
| XML | | | 
