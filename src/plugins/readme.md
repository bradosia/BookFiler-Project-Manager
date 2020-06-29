# Plugins

## Interface

Plugins are exported like so:

```cpp
extern "C" BOOST_SYMBOL_EXPORT pluginInterface plugin;
pluginInterface plugin;
```

## Plugin Manager

QT has a plugin system and UI, but to prevent overdependence on QT this system won't be used. Plugin loading logic is in the PluginManager class. Plugins are automatically loaded like so:

```cpp
PluginManager pluginManagerObject;
pluginManagerObject.loadPlugins("plugins_directory");
for (Plugin pluginObj : pluginManagerObject.pluginList()){
  pluginObj.getPluginName();
}
```
In reality, the interface needs to be specified for each plugin.

```cpp
pluginManagerObject.addPluginInterface(InterfaceName,"InterfaceName");
```
