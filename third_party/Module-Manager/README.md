# Module Manager
A simple dynamic library module manager and loader. Easily define modules and load them from a module directory.

# Dependencies
* boost
* c++17

# Example Usage
main.cpp
```cpp
#include "ModuleManager/ModuleManager.hpp"
#include "ModuleInterface.hpp"
using namespace bradosia;

int main(){
  ModuleManager moduleManagerObj;
  moduleManagerObj.addModuleInterface<ModuleInterface>("moduleName");
  moduleManagerObj.loadModules("modules_directory");
  boost::shared_ptr<ModuleInterface> module = moduleManagerObj.getModule<ModuleInterface>("moduleName");
  module->testModule();
}
```

ModuleInterface.hpp
```cpp
class ModuleInterface {
public:
  virtual void testModule() = 0;
};
```

module.hpp source for "modules_directory/module.dll"
```cpp
#include <boost/config.hpp> // for BOOST_SYMBOL_EXPORT
#include "moduleInterface.hpp"

namespace ModuleNamespace {
class ModuleClass : public ModuleInterface {
  void testModule(){
    printf("Hello World!");
  }
}

extern "C" BOOST_SYMBOL_EXPORT ModuleClass moduleName;
ModuleClass moduleName;
}
```

# Build
It is recommended in your project directory to place this library folder at `third_party/Module-Manager/`

## Cmake

```cmake
add_subdirectory(third_party/Module-Manager)

target_include_directories(${YOUR_BINARY} PRIVATE
  third_party/Module-Manager/include
)

target_link_directories(${YOUR_BINARY} PRIVATE
   third_party/Module-Manager/lib
)

target_link_libraries(${YOUR_BINARY} PRIVATE
  ModuleManager
)
```

