/*
 * @name Filesystem Database Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief filesystem database and utilities
 */

#ifndef FSDB_FILE_TABLE_INCLUDE_H
#define FSDB_FILE_TABLE_INCLUDE_H

// C++
#include <memory>
#include <string>
#include <unordered_map>

// Local Project
#include "config.hpp"

namespace FSDB {
namespace filesystem {

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

/* FileTableData
 * This class is not a model. Don't add parent and child
 * relationships.
 * The table is public for faster direct access.
 * Obfuscating the interface would cause extra overhead.
 * The user should already know the internal implementation.
 * This class is for medium sized mission critical applications so
 * performance is needed more than pretty interfaces.
 */
class FileTableData {
public:
  FileTableData(){};
  ~FileTableData(){};
  std::string parentPathId;
  std::unordered_map<int, std::shared_ptr<FileTableRow>> table;
  /* It may be faster to get the row then access the column data
   */
  std::shared_ptr<FileTableRow> getRow(int row) { return table.at(row); }
};

} // namespace filesystem
} // namespace FSDB

#endif // FSDB_FILE_TABLE_INCLUDE_H
