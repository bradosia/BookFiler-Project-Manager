/*
 * @name Module Manager
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief Manages shared library modules
 */

#ifndef BRADOSIA_MODULE_MANAGER_H
#define BRADOSIA_MODULE_MANAGER_H

// c++
#include <iostream>
#include <memory>
#include <string>

/* boost 1.72.0
 * License: Boost Software License (similar to BSD and MIT)
 */
#include "boost/filesystem.hpp"
#include <boost/dll/import.hpp> // for import_alias
#include <boost/shared_ptr.hpp>

namespace bradosia {

template <class T> std::shared_ptr<T> to_std(const boost::shared_ptr<T> &p) {
  return std::shared_ptr<T>(p.get(), [p](T *) mutable {
    // p.reset();
  });
}

template <class T> boost::shared_ptr<T> to_boost(const std::shared_ptr<T> &p) {
  return boost::shared_ptr<T>(p.get(), [p](T *) mutable { p.reset(); });
}

class InterfaceMethodsBase {
public:
  std::string moduleName;
  InterfaceMethodsBase(std::string s) { moduleName = s; }
  ~InterfaceMethodsBase() {}
  virtual void addPath(boost::filesystem::path lib_path) = 0;
};

template <class T> class InterfaceMethods : public InterfaceMethodsBase {
public:
  InterfaceMethods(std::string s) : InterfaceMethodsBase(s) {}
  ~InterfaceMethods() {}
  std::vector<boost::shared_ptr<T>> modulePtrs;
  void addPath(boost::filesystem::path lib_path) {
    std::cout << "PLUGIN: Loading " << lib_path << "\n";
    boost::shared_ptr<T> module;
    try {
      module = boost::dll::import<T>(lib_path, moduleName,
                                     boost::dll::load_mode::default_mode);
    } catch (boost::system::system_error e) {
      std::cout << "addPath(" << lib_path << ") EXCEPTION: " << e.what()
                << "\n";
    } catch (...) {
      std::cout << "addPath(" << lib_path << ") EXCEPTION\n";
    }
    if (module) {
      std::cout << "addPath(" << lib_path << ") SUCCESS\n";
      modulePtrs.push_back(module);
    }
  }
};

class ModuleManager {
private:
  std::unordered_map<std::string, InterfaceMethodsBase *> interfaceMap;

public:
  template <class T> void addModuleInterface(std::string moduleName) {
    InterfaceMethods<T> *interface = new InterfaceMethods<T>(moduleName);
    interfaceMap.insert({moduleName, interface});
  }

  void loadModules(std::string directoryPathStr) {
    for (auto &p :
         boost::filesystem::recursive_directory_iterator(directoryPathStr)) {
      std::cout << "MODULE: File Found " << p.path() << "\n";
      std::cout << "path extension " << p.path().extension().string() << "\n";
      std::string pathStr = p.path().string();
      /* On linux the dynamic library extensions are libMyLibrary.so.1.0.0
       * Thus, we use find instead of substr matching
       */
      if (boost::filesystem::is_regular_file(p) &&
          (pathStr.find(".dll") != std::string::npos ||
           pathStr.find(".dylib") != std::string::npos ||
           pathStr.find(".so") != std::string::npos)) {
        for (auto pairs : interfaceMap) {
          pairs.second->addPath(p.path());
        }
      }
    }
  }

  template <class T> std::shared_ptr<T> getModule(std::string moduleName) {
    InterfaceMethods<T> *interface =
        dynamic_cast<InterfaceMethods<T> *>(interfaceMap.at(moduleName));
    if (interface->modulePtrs.empty()) {
      return nullptr;
    }
    return to_std(interface->modulePtrs.front());
  }
};

} // namespace bradosia

#endif
// end BRADOSIA_MODULE_MANAGER_H
