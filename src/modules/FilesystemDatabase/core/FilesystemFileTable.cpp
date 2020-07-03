/*
 * @name Filesystem Database Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief filesystem database and utilities
 */

// local project
#include "FilesystemFileTable.hpp"

namespace FSDB {
namespace filesystem {

static int callback(void *data, int argc, char **argv, char **azColName) {
  int i;
  for (i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

FileTable::FileTable() {}
FileTable::~FileTable() {}
bool FileTable::open() {
  // full path to binary file
  boost::filesystem::path programPath = boost::dll::program_location();
#if FILESYSTEM_FILE_TABLE_DEBUG
  std::cout << "FileTable::open() program_location()=" << programPath << "\n";
#endif
  boost::filesystem::path databasePath =
      programPath.parent_path() / databaseFileName;
  /* Open database */
  int rc = sqlite3_open(databasePath.string().c_str(), &db);
  if (rc) {
#if FILESYSTEM_FILE_TABLE_DEBUG
    std::cout << "Can't open database:" << sqlite3_errmsg(db) << "\n";
#endif
    return false;
  }
  databaseOpenFlag = true;
  return true;
}

bool FileTable::createTable() {
  if (!databaseOpenFlag) {
    return false;
  }
  char *zErrMsg = 0;
  /* Create SQL statement */
  std::string sql = "CREATE TABLE IF NOT EXISTS FileInfo(";
  std::vector<std::string> columnLines;
  for (auto colName : textColumns) {
    if (colName == "pathId") {
      columnLines.push_back(colName + " TEXT PRIMARY KEY");
    } else {
      columnLines.push_back(colName + " TEXT NOT NULL");
    }
  }
  for (auto colName : intColumns) {
    columnLines.push_back(colName + " INTEGER NOT NULL");
  }
  sql.append(boost::algorithm::join(columnLines, ",")).append(");");

  std::cout << "FileTable::open() SQL:\n" << sql << "\n";
  /* Execute SQL statement */
  int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "FileTable::open() SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return false;
  }
  fprintf(stdout, "FileTable::open() Table created successfully\n");
  return true;
}

void FileTable::getData(
    std::string parentPathId,
    std::function<void(std::shared_ptr<FileTableData>)> callback) {
  std::cout << "FSDB::filesystem::FileTable::getData(" << parentPathId << ")\n";
  std::shared_ptr<FileTableData> fileTableDataPtr =
      std::make_shared<FileTableData>();
  fileTableDataPtr->parentPathId = parentPathId;
  if (!databaseOpenFlag) {
    callback(fileTableDataPtr);
    return;
  }
  std::string sql =
      "SELECT * FROM FileInfo WHERE `parentPathId`='" + parentPathId + "'";
  std::cout << "FileTable::getData SQL:\n" << sql << "\n";
  /* Execute SQL statement */
  sqlite3_stmt *stmt = NULL;
  int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    std::cout << "FSDB::filesystem::FileTable::getData(" << parentPathId
              << ")\nERROR: " << sqlite3_errstr(rc) << "\n";
    callback(fileTableDataPtr);
    return;
  }
  int rowCount = 0;
  rc = sqlite3_step(stmt);
  while (rc != SQLITE_DONE && rc != SQLITE_OK) {
    std::shared_ptr<FileTableRow> rowPtr = std::make_shared<FileTableRow>();
    rowPtr->pathId = std::string(
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
    rowPtr->parentPathId = std::string(
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
    rowPtr->drive = std::string(
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
    rowPtr->fileType = sqlite3_column_int(stmt, 3);
    rowPtr->fileSize = sqlite3_column_int(stmt, 4);
    rowPtr->writeTimeLast = sqlite3_column_int(stmt, 5);
    rowPtr->flags = sqlite3_column_int(stmt, 6);
    fileTableDataPtr->table.insert({rowCount, rowPtr});
#if FILESYSTEM_FILE_TABLE_GET_DATA_DEBUG
    std::cout << "FileTable::getData() row=" << rowCount << "\n";
#endif
    rc = sqlite3_step(stmt);
    rowCount++;
  }
  rc = sqlite3_finalize(stmt);
  std::cout << "FSDB::filesystem::FileTable::getData(" << parentPathId
            << ") finalized\n";
  callback(fileTableDataPtr);
}

void FileTable::setDatabaseFileName(std::string fileName) {
  databaseFileName = fileName;
}

void FileTable::insertDatabaseByJson(rapidjson::Value &data) {
  if (data.IsArray()) {
    // rapidjson uses SizeType instead of size_t.
    rapidjson::SizeType n = data.Size();
    // iterate children
    for (rapidjson::SizeType i = 0; i < n; i++) {
      if (data[i].IsObject()) {
        // The minimum is field is pathId
        if (data[i].HasMember("pathId")) {
          std::string sql = "INSERT INTO FileInfo(";
          std::vector<std::string> insertColumnNames;
          std::vector<std::string> insertColumnData;
          for (std::string colName : textColumns) {
            auto result = data[i].FindMember(colName.c_str());
            if (result != data[i].MemberEnd()) {
              if (result->value.IsString()) {
                insertColumnNames.push_back(colName);
                insertColumnData.push_back(
                    "'" + std::string(result->value.GetString()) + "'");
              }
            }
          }
          for (std::string colName : intColumns) {
            auto result = data[i].FindMember(colName.c_str());
            if (result != data[i].MemberEnd()) {
              if (result->value.IsInt()) {
                insertColumnNames.push_back(colName);
                insertColumnData.push_back(
                    std::to_string(result->value.GetInt()));
              }
            }
          }
          sql.append(boost::algorithm::join(insertColumnNames, ","))
              .append(") VALUES(");
          sql.append(boost::algorithm::join(insertColumnData, ",") + ")");
          std::cout << "FileTable::insertDatabaseByJson SQL:\n" << sql << "\n";
          /* Execute SQL statement */
          sqlite3_stmt *stmt = NULL;
          int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
          if (rc != SQLITE_OK) {
            std::cout << "FileTable::insertDatabaseByJson\nERROR: "
                      << sqlite3_errstr(rc) << "\n";
          } else {
            int rowCount = 0;
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_OK) {
              std::cout << "FileTable::insertDatabaseByJson\nERROR " << rc
                        << ": " << sqlite3_errstr(rc) << "\n";
            }
            rc = sqlite3_finalize(stmt);
          }
        }
      }
    }
    std::cout << "FileTable::insertDatabaseByJson done\n";
  }
}

} // namespace filesystem
} // namespace FSDB
