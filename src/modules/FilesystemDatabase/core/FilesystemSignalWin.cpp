/*
 * @name Filesystem Database Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief filesystem database and utilities
 */

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifndef FSDB_FILESYSTEM_SIGNAL_CPP
#define FSDB_FILESYSTEM_SIGNAL_CPP

// C++
#include <iostream>
#include <memory>

// local project
#include "FilesystemSignalWin.hpp"

namespace FSDB {
namespace filesystem {

/* Designing the signaling for Windows has many methods:
 * This has a good outline of the common methods to using ReadDirectoryChangesW
 * https://qualapps.blogspot.com/2010/05/understanding-readdirectorychangesw.html
 * This code use the simple method
 * ReadDirectoryChangesW is blocking a thread and
 * reports its data using boost signals2
 */
Signal::Signal() {
  /* this is pretty arbitrary since the file lengths are unknown
   * Let's arbitraily hold at least 3 entries with a path length MAX_PATH
   * sizeof(struct _FILE_NOTIFY_INFORMATION) = 16 bytes
   * MAX_PATH=260 wchar_t = 520 bytes
   * so each _FILE_NOTIFY_INFORMATION could be 16 + 520 = 436 bytes
   * 3 of these = 1308 bytes
   * In my testing I have never seen more than two events in one blocking read
   * and that typically only happens for a file rename
   */
  bufferSize = 1308;
}

Signal::~Signal() {
  for (auto &handleIt : handleMap) {
    handleIt.second->threadRunFlag = false;
    CloseHandle(handleIt.second->fileHandle);
    handleIt.second->threadRunFlag = false;
  }
}

void Signal::watch(HANDLE fileHandle, std::shared_ptr<HandleInfo> handleInfo,
                   boost::filesystem::path watchPath) {
  /* A DWORD is 4 bytes or 32-bits
   */
  DWORD buffer[bufferSize];
  unsigned int bufferBytes = bufferSize * 4;
  /* What will happen when the instance is deleted?
   */
  while (handleInfo->threadRunFlag) {
    // reset variables and buffer
    DWORD bytesRead = 0;
    DWORD lpBytesReturned = 0;
    DWORD *bufferPtr = buffer;
    memset(buffer, 0, bufferBytes);

    // blocks until there is something to read
    bool flag = ReadDirectoryChangesW(
        fileHandle, buffer, bufferBytes, TRUE,
        FILE_NOTIFY_CHANGE_SECURITY | FILE_NOTIFY_CHANGE_CREATION |
            FILE_NOTIFY_CHANGE_LAST_ACCESS | FILE_NOTIFY_CHANGE_LAST_WRITE |
            FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_ATTRIBUTES |
            FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_FILE_NAME,
        &lpBytesReturned, NULL, nullptr);

#if FILESYSTEM_SIGNAL_DEBUG
    std::cout << "Signal::watch(" << fileHandle
              << ") lpBytesReturned=" << lpBytesReturned
              << " struct size=" << sizeof(struct _FILE_NOTIFY_INFORMATION)
              << "\n";
#endif
    /* checking for error
     */
    if (lpBytesReturned >= 0 && handleInfo->threadRunFlag) {
      /* actually read return the list of change events happens. Here, read the
       * change event one by one and process it accordingly.
       */
      while (bytesRead < lpBytesReturned) {
        struct _FILE_NOTIFY_INFORMATION *event =
            reinterpret_cast<struct _FILE_NOTIFY_INFORMATION *>(bufferPtr);
        boost::filesystem::path eventPath(
            std::wstring(event->FileName, event->FileNameLength / 2));
        boost::filesystem::path eventFullPath(watchPath / eventPath);
        std::shared_ptr<SignalEvent> sigEvent = std::make_unique<SignalEvent>();
        sigEvent->path = eventFullPath.generic_wstring();
        if (event->Action == FILE_ACTION_ADDED) {
          sigEvent->type = signalEventType::fileCreate;
        } else if (event->Action == FILE_ACTION_REMOVED) {
          sigEvent->type = signalEventType::fileDelete;
        } else if (event->Action == FILE_ACTION_MODIFIED) {
          sigEvent->type = signalEventType::fileDataChange;
        } else if (event->Action == FILE_ACTION_RENAMED_OLD_NAME) {
          sigEvent->type = signalEventType::fileNameChange;
        } else if (event->Action == FILE_ACTION_RENAMED_NEW_NAME) {
          sigEvent->type = signalEventType::fileNameChange;
        }
        // signal
        (*sig)(sigEvent);
#if FILESYSTEM_SIGNAL_DEBUG
        std::cout << "Signal::watch(" << fileHandle
                  << ") NextEntryOffset=" << event->NextEntryOffset
                  << " FileNameLength=" << event->FileNameLength << "\n";
#endif
        // A value of zero indicates that this is the last record.
        if (event->NextEntryOffset == 0) {
          break;
        }
        bufferPtr += event->NextEntryOffset / 4;
        bytesRead += event->NextEntryOffset / 4;
      }
    } else {
      // error
    }
  }
}

bool Signal::addWatch(std::wstring pathName) {
  boost::filesystem::path watchPath(pathName);
#if FILESYSTEM_SIGNAL_DEBUG
  std::cout << "Signal::addWatch(" << watchPath << ") start\n";
#endif
  HANDLE fileHandle =
      CreateFileW(watchPath.wstring().c_str(), FILE_LIST_DIRECTORY,
                  FILE_SHARE_READ | FILE_SHARE_DELETE | FILE_SHARE_WRITE, NULL,
                  OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
  if (fileHandle == INVALID_HANDLE_VALUE) {
    return false;
  }
#if FILESYSTEM_SIGNAL_DEBUG
  std::cout << "Signal::addWatch(" << watchPath
            << ") WINAPI CreateFileW success\n";
#endif
  std::shared_ptr<HandleInfo> handleInfo =
      std::make_shared<HandleInfo>(fileHandle, true);
  /* Each watch on a separate thread
   */
  std::shared_ptr<std::thread> watcherThread = std::make_shared<std::thread>(
      &Signal::watch, this, fileHandle, handleInfo, watchPath);
  watcherThread->detach();

  handleMap.insert({fileHandle, handleInfo});
  return true;
}

} // namespace filesystem
} // namespace FSDB

#endif // FSDB_FILESYSTEM_SIGNAL_H
#endif
