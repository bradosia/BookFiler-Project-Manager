/*
 * @name Filesystem Database Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief filesystem database and utilities
 */

#ifndef FSDB_FILESYSTEM_SIGNAL_NATIVE_H
#define FSDB_FILESYSTEM_SIGNAL_NATIVE_H

// C++
#include <thread>
#include <unordered_map>

// WINAPI
#include <windows.h>

// Boost
#include <boost/signals2.hpp>

// Local Project
#include "FilesystemSignalBase.hpp"

namespace FSDB {
namespace filesystem {

/* FSDB::filesystem::Signal class
 *
 */
class Signal : public SignalBase {
private:
  class HandleInfo {
  public:
    HANDLE fileHandle;
    bool threadRunFlag = false;
    HandleInfo(HANDLE fileHandle_, bool threadRunFlag_)
        : fileHandle(fileHandle_), threadRunFlag(threadRunFlag_){};
    ~HandleInfo(){};
  };

  std::unordered_map<HANDLE, std::shared_ptr<HandleInfo>> handleMap;
  unsigned int bufferSize;

public:
  Signal();
  ~Signal();
  void watch(HANDLE, std::shared_ptr<HandleInfo>, boost::filesystem::path);
  bool addWatch(std::wstring pathName);
};

} // namespace filesystem
} // namespace FSDB

#endif // FSDB_FILESYSTEM_SIGNAL_NATIVE_H
