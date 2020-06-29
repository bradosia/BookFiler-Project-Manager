# Settings Manager
Manage application settings and preferences with a JSON settings file.

This class is for merging settings from multiple modules then deploying them back to the modules. It enables users to only need one settings file with only applicable settings deployed to each module.

# Usage

```cpp
// get JSONdata encoded settings from modules
if(merge(JSONdata, moduleCallbackMap)){
// deploy application primary settings
deployFile("settings.json");
}
```

# JSON parsing
RapidJSON library is used. File parsing uses parse flags:

| Parse Flag | Description |
|:--|:--|
|rapidjson::kParseCommentsFlag|Allow one-line (//) and multi-line (/**‍/) comments.|
|rapidjson::kParseTrailingCommasFlag|Allow trailing commas at the end of objects and arrays.|
See https://rapidjson.org/namespacerapidjson.html#a81379eb4e94a0386d71d15fda882ebc9

# Settings Storage Data Structure
`rapidjson::Document` is used to store settings. 

# Why is RapidJSON used?
The hjson library was also considered as an option because it allows more flexible json settings file. rapidJSON is often used as a high performance JSON parsing. Even though each JSON library serves different uses, using two different JSON libraries seemed redundant and confusing.

# Build
It is recommended in your project directory to place this library folder at `third_party/Settings-Manager/`

## Cmake

```cmake
add_subdirectory(third_party/Settings-Manager)

target_include_directories(${YOUR_BINARY} PRIVATE
  third_party/Settings-Manager/include
)

target_link_directories(${YOUR_BINARY} PRIVATE
   third_party/Settings-Manager/lib
)

target_link_libraries(${YOUR_BINARY} PRIVATE
  SettingsManager
)
``` 