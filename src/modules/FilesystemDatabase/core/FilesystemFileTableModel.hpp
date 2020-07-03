/*
 * @name Filesystem Database Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief filesystem database and utilities
 */

#ifndef FSDB_FILE_TABLE_DATA_H
#define FSDB_FILE_TABLE_DATA_H

// Config
#include "config.hpp"

// C++
#include <memory>
#include <string>

// Local Project
#include "FilesystemFileTableInclude.hpp"

namespace FSDB {
namespace filesystem {

class FileModel {
public:
  FileModel(){};
  ~FileModel(){};
  std::shared_ptr<FileTableData> rootTable;
};

} // namespace filesystem
} // namespace FSDB

#endif // FSDB_FILE_TABLE_DATA_H
