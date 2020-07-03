/*
 * @name File Tree Pane Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief QT5 Widget for viewing a file tree
 */

#ifndef FILE_TREE_PANE_MODULE_H
#define FILE_TREE_PANE_MODULE_H

// config
#include "core/config.hpp"

// c++17
//#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>

/* boost 1.72.0
 * License: Boost Software License (similar to BSD and MIT)
 */
#include <boost/config.hpp> // for BOOST_SYMBOL_EXPORT

/* rapidjson v1.1 (2016-8-25)
 * Developed by Tencent
 * License: MITs
 */
#include <rapidjson/document.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/reader.h> // rapidjson::ParseResult
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

/* QT 5.13.2-1
 * License: LGPLv3
 */
#include <QTreeView>
#include <QWidget>

// Local Project
#include "Interface.hpp"
#include "UI/MainWidget.hpp"
#include "UI/WidgetItemDelegate.hpp"
#include "UI/WidgetModel.hpp"

/*
 * FileTreePane
 */
namespace FileTreePane {

/*
 * Plugin uses the MVC design pattern.
 * This is the controller, the view is a QT widget, and the model is the API
 * storage
 */
class ModuleExport : public ModuleInterface {
private:
  std::shared_ptr<WidgetModel> treeModel;
  std::shared_ptr<WidgetItemDelegate> filesystemItemDelegate;
  std::shared_ptr<MainWidget> treeView;

public:
  ModuleExport();
  ~ModuleExport(){};

  void init() { printf("File Tree Pane Module: init()\n"); }
  std::shared_ptr<QWidget> getWidget();
  void treeSetJSON(std::shared_ptr<rapidjson::Document> data);
  void
  registerSettings(std::shared_ptr<rapidjson::Document> moduleRequest,
                   std::shared_ptr<std::unordered_map<
                       std::string, std::function<void(std::shared_ptr<rapidjson::Document>)>>>
                       moduleCallbackMap);
  void pathSetByJSON(std::shared_ptr<rapidjson::Document> data);


};

// Exporting `my_namespace::module` variable with alias name `module`
// (Has the same effect as `BOOST_DLL_ALIAS(my_namespace::module, module)`)
extern "C" BOOST_SYMBOL_EXPORT ModuleExport fileTreePaneModule;
ModuleExport fileTreePaneModule;

} // namespace FileTreePane

#endif
// end FILE_TREE_PANE_MODULE_H
