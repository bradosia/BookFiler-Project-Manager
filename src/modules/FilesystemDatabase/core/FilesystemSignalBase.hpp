/*
 * @name Filesystem Database Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief filesystem database and utilities
 */

#ifndef FSDB_FILESYSTEM_SIGNAL_BASE_H
#define FSDB_FILESYSTEM_SIGNAL_BASE_H

// Config
#include "config.hpp"

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
#include "Filesystem.hpp"
#include "FilesystemSignalInclude.hpp"

namespace FSDB {
namespace filesystem {

/* FSDB::filesystem::Signal class
 *
 * determine the event change similar to:
 * POSIX: read(fd, buffer, EVENT_BUF_LEN);
 * WINAPI: WaitForMultipleObjects(2, dwChangeHandles, FALSE, INFINITE);
 *
 * No removeWatch() method, instead create a new signal.
 */
class SignalBase {
protected:
  std::shared_ptr<boost::signals2::signal<void(std::shared_ptr<SignalEvent>)>>
      sig;
  bool initError = false;

public:
  /* Similar to:
   * POSIX: inotify_init()
   * WINAPI:
   */
  SignalBase() {
    sig = std::make_shared<
        boost::signals2::signal<void(std::shared_ptr<SignalEvent>)>>();
  }
  ~SignalBase(){};
  std::shared_ptr<boost::signals2::signal<void(std::shared_ptr<SignalEvent>)>>
  getSignal() {
    return sig;
  }
  void connect(std::function<void(std::shared_ptr<SignalEvent>)> callback) {
    sig->connect(callback);
  }
  /* Similar to:
   * POSIX: inotify_add_watch
   * WINAPI: FindFirstChangeNotification
   */
  virtual bool addWatch(std::wstring pathName) = 0;
};

} // namespace filesystem
} // namespace FSDB

#endif // FSDB_FILESYSTEM_SIGNAL_BASE_H
