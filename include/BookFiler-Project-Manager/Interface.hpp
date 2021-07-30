/*
 * @name File Tree Pane Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief QT5 Widget for viewing a file tree
 */

#ifndef BOOKFILER_FILE_TREE_PANE_INTERFACE_H
#define BOOKFILER_FILE_TREE_PANE_INTERFACE_H

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

/*
 * bookfiler = BookFiler™
 */
namespace bookfiler {

namespace filesystem {

#ifndef BOOKFILER_FSDB_FILE_TABLE_INCLUDE_H
#define BOOKFILER_FSDB_FILE_TABLE_INCLUDE_H
enum class FileTableRowFlags {
  expanded = 1,
  collapsed = 0,
};

class FileTableRow {
public:
  FileTableRow(){};
  ~FileTableRow(){};
  std::string pathId, parentPathId, drive;
  int fileType, fileSize, writeTimeLast, flags;
};

class FileTableData {
public:
  FileTableData(){};
  ~FileTableData(){};
  std::string parentPathId;
  std::unordered_map<int, std::shared_ptr<FileTableRow>> table;
  std::shared_ptr<FileTableRow> getRow(int row) { return table.at(row); }
};
#endif // BOOKFILER_FSDB_FILE_TABLE_INCLUDE_H

} // namespace filesystem

#ifndef BOOKFILER_WIDGET_H
#define BOOKFILER_WIDGET_H
class WidgetData {
public:
  int displaySizeX, displaySizeY, displayFramebufferScale, mousePosX, mousePosY,
      mouseWheelX, mouseWheelY;
  double deltaTime;
  std::string addInputCharacter;
  bool keyCtrl, keyShift, keyAlt, keySuper, windowFocused;
  std::unordered_map<int, bool> mouseDown;
  std::unordered_map<int, bool> keysDown;
  void (*setClipboardTextFn)(void *, const char *);
  const char *(*getClipboardTextFn)(void *);
  void *winId;
};

class WidgetMouseEvent {
public:
  int x, y;
};

class WidgetKeyEvent {
public:
  int x, y;
};
#endif // end BOOKFILER_WIDGET_H

class FileTreePaneWidget {
public:
  virtual bool initGraphics(std::shared_ptr<bookfiler::WidgetData>) = 0;
  virtual bool render(std::shared_ptr<bookfiler::WidgetData>) = 0;
  /* Slots */
  std::function<void(std::shared_ptr<bookfiler::filesystem::FileTableData>)>
      viewTreeDataSlot;
  std::function<void(std::shared_ptr<rapidjson::Document>)> viewTreeJsonSlot;
  /* Signals
   * How to connect in your code:
   * class myDataClass;
   * void MyNamespace::MyClass(std::shared_ptr<myDataClass>);
   * std::function<void(std::shared_ptr<myDataClass>)> slot
   * = std::bind(&MyNamespace::MyClass, this, std::placeholders::_1);
   * signal.connect(slot);
   *
   * Do not need to pass a callback since slots may activate signals of their
   * own.
   */
  boost::signals2::signal<void()> getDirectorySignal;
};

class FileTreePaneInterface {
public:
  /* need to make virtual method() = 0
   * because of error:
   * undefined reference to `vtable for FileTreePane::ModuleInterface'
   */
  virtual void init() = 0;
  virtual std::shared_ptr<FileTreePaneWidget> newWidget() = 0;
  virtual std::shared_ptr<FileTreePaneWidget> getWidget(unsigned int) = 0;
  virtual bool deleteWidget(unsigned int) = 0;
  virtual void registerSettings(
      std::shared_ptr<rapidjson::Document>,
      std::shared_ptr<std::unordered_map<
          std::string,
          std::function<void(std::shared_ptr<rapidjson::Document>)>>>) = 0;
};

} // namespace bookfiler

#endif
// end BOOKFILER_FILE_TREE_PANE_INTERFACE_H
