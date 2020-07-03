# Modules

## Interface

Modules are exported like so:

```cpp
extern "C" BOOST_SYMBOL_EXPORT ModuleInterface Module;
ModuleInterface Module;
```

## Module Manager

QT has a Module system and UI, but to prevent overdependence on QT this system won't be used. Module loading logic is in the ModuleManager class. Modules are automatically loaded like so:

```cpp
ModuleManager moduleManagerObject;
moduleManagerObject.loadModules("Modules_directory");
for (Module ModuleObj : mduleManagerObject.ModuleList()){
  ModuleObj.getModuleName();
}
```
In reality, the interface needs to be specified for each Module.

```cpp
moduleManagerObject.addModuleInterface(InterfaceName,"InterfaceName");
```
