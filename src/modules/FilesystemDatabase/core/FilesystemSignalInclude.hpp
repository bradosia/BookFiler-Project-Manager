/*
 * @name Filesystem Database Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief filesystem database and utilities
 */

#ifndef FSDB_FILESYSTEM_SIGNAL_INCLUDE_H
#define FSDB_FILESYSTEM_SIGNAL_INCLUDE_H

// C++
#include <codecvt>
#include <functional>
#include <locale> // wstring_convert
#include <memory>
#include <string>

/* boost 1.72.0
 * License: Boost Software License (similar to BSD and MIT)
 */
#include <boost/signals2.hpp>

// Local Project
#include "config.hpp"
#include "Filesystem.hpp"

namespace FSDB {
namespace filesystem {

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

/* Similar to:
 * POSIX: struct inotify_event
 * WINAPI: struct _FILE_NOTIFY_INFORMATION
 */
class SignalEvent {
public:
  signalEventType type;
  std::wstring path;
};

} // namespace filesystem
} // namespace FSDB

#endif // FSDB_FILESYSTEM_SIGNAL_INCLUDE_H
