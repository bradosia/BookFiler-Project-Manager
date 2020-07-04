/*
 * @name File Tree Pane Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief QT5 Widget for viewing a file tree
 */

#ifndef FILE_TREE_PANE_INTERFACE_H
#define FILE_TREE_PANE_INTERFACE_H

// c++17
#include <functional>
#include <memory>
#include <unordered_map>

/* rapidjson v1.1 (2016-8-25)
 * Developed by Tencent
 * License: MITs
 */
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>

/* boost 1.72.0
 * License: Boost Software License (similar to BSD and MIT)
 */
#include <boost/signals2.hpp>

/* QT 5.13.2
 * License: LGPLv3
 */
#include <QWidget>

// FilesystemDatabase Module
#include "../FilesystemDatabase/Interface.hpp"

/*
 * FileTreePane
 */
namespace FileTreePane {

class ModuleInterface {
public:
  /* need to make virtual method() = 0
   * because of error:
   * undefined reference to `vtable for FileTreePane::ModuleInterface'
   */
  virtual void init() = 0;
  virtual std::shared_ptr<QWidget> getWidget() = 0;
  virtual void registerSettings(
      std::shared_ptr<rapidjson::Document>,
      std::shared_ptr<std::unordered_map<
          std::string,
          std::function<void(std::shared_ptr<rapidjson::Document>)>>>) = 0;
  /* Set the tree with JSON
   */
  virtual void treeSetJSON(std::shared_ptr<rapidjson::Document> data) = 0;
  /* ====SIGNALS====
   * How to connect in your code:
   * class myDataClass;
   * void MyNamespace::MyClass(std::shared_ptr<myDataClass>);
   * std::function<void(std::shared_ptr<myDataClass>)> callBack
   * = std::bind(&MyNamespace::MyClass, this, std::placeholders::_1);
   * signal.connect(callBack);
   */
  /* getDirectorySignal
   * The callback arguments are the same as those in SQLite 3
   */
  std::shared_ptr<boost::signals2::signal<void(
      std::string, std::function<void(std::shared_ptr<FSDB::filesystem::FileTableData>)>)>>
      getDirectorySignal;
};

} // namespace FileTreePane

#endif
// end FILE_TREE_PANE_INTERFACE_H
