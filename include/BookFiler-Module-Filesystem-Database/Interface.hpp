/*
 * @name Filesystem Database Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief filesystem database and utilities
 */

#ifndef BOOKFILER_FILESYSTEM_DATABASE_INTERFACE_H
#define BOOKFILER_FILESYSTEM_DATABASE_H

// C++17
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

/* rapidjson v1.1 (2016-8-25)
 * Developed by Tencent
 * License: MITs
 */
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>

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

#ifndef FSDB_FILESYSTEM_SIGNAL_INCLUDE_H
#define FSDB_FILESYSTEM_SIGNAL_INCLUDE_H
enum class signalEventType {
  fileCreate,
  fileDelete,
  fileNameChange,
  fileDateChange,
  fileDataChange,
  dirCreate,
  dirDelete,
  dirNameChange
};

class SignalEvent {
public:
  signalEventType type;
  std::wstring path;
};
#endif // FSDB_FILESYSTEM_SIGNAL_INCLUDE_H

} // namespace filesystem

class FileSystemDatabaseInterface {
public:
  virtual void init() = 0;
  virtual void registerSettings(
      std::shared_ptr<rapidjson::Document>,
      std::shared_ptr<std::unordered_map<
          std::string,
          std::function<void(std::shared_ptr<rapidjson::Document>)>>>) = 0;
  /* Must be virtual or the linker compainsD
   */
  virtual void getDirectorySlot(
      std::string,
      std::function<void(std::shared_ptr<filesystem::FileTableData>)>) = 0;
  virtual int newModel() = 0;
  virtual int newSignal() = 0;
  virtual bool connectSignal(
      int, std::function<void(std::shared_ptr<filesystem::SignalEvent>)>) = 0;
  /* Should we use a boost::filesystem::path as an argument?
   * This would force the host to the module to link boost
   * instead we settle on a std::wstring to represent a path.
   * Maybe a UTF-8 encoded std::string would be better because most paths
   * are usually in western languages.
   * So we add both
   */
  virtual bool watchSignal(int, std::wstring) = 0;
  virtual bool watchSignal(int, std::string) = 0;
};

} // namespace bookfiler

#endif
// end BOOKFILER_FILESYSTEM_DATABASE_H
