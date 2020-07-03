/*
 * @name Filesystem Database Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief filesystem database and utilities
 */

#ifndef FSDB_FILE_TABLE_H
#define FSDB_FILE_TABLE_H

// config
#include "config.hpp"

// C++
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <thread>
#include <unordered_map>

/* boost 1.72.0
 * License: Boost Software License (similar to BSD and MIT)
 */
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/dll/runtime_symbol_info.hpp>

/* rapidjson v1.1 (2016-8-25)
 * Developed by Tencent
 * License: MITs
 */
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>

/* SQLite 3.31.1
 * License: Public domain
 */
#include <sqlite3.h>

// Local Project
#include "FilesystemFileTableModel.hpp"

namespace FSDB {
namespace filesystem {

class FileTable {
public:
  FileTable();
  ~FileTable();
  bool open();
  bool createTable();
  void getData(std::string pathStr,
               std::function<void(std::shared_ptr<FileTableData>)> callback);
  void setDatabaseFileName(std::string fileName);
  void insertDatabaseByJson(rapidjson::Value &data);

private:
  sqlite3 *db;
  bool databaseOpenFlag = false;
  std::string databaseFileName = "default.db";
  std::vector<std::string> textColumns = {"pathId", "parentPathId", "drive"};
  std::vector<std::string> intColumns = {"fileType", "fileSize",
                                         "writeTimeLast", "flags"};
};

} // namespace filesystem
} // namespace FSDB

#endif // DISK_USAGE_FILESYSTEM_H
