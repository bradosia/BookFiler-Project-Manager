/*
 * @name Filesystem Database Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief filesystem database and utilities
 */

#ifndef FSDB_FILESYSTEM_H
#define FSDB_FILESYSTEM_H

// C++
#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale> // wstring_convert
#include <queue>
#include <thread>
#include <unordered_map>

/* boost 1.72.0
 * License: Boost Software License (similar to BSD and MIT)
 */
#include "boost/filesystem.hpp"

namespace FSDB {
namespace filesystem {

/* Idea: In windows, use the ProgramData folder to save data
 * KNOWNFOLDERID: FOLDERID_ProgramData
 * Edit: It is better to keep the data in the same folder as the binary
 * to reduce fragmenting of files for easy uninstallation
 */

void traverse(
    boost::filesystem::path pathInitial,
    std::shared_ptr<std::unordered_map<std::wstring, bool>> visitedPtr);
std::string toUTF8String(std::wstring);
// UT8 string to UTF16 wstring
std::wstring toUTF16Wstring(std::string);

} // namespace filesystem
} // namespace FSDB

#endif // DISK_USAGE_FILESYSTEM_H
