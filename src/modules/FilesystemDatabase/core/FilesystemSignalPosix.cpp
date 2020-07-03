/*
 * @name Filesystem Database Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief filesystem database and utilities
 */

#ifndef FSDB_FILESYSTEM_SIGNAL_CPP
#define FSDB_FILESYSTEM_SIGNAL_CPP

// C++
#include <memory>

// POSIX
#include <sys/inotify.h>
#include <sys/types.h>

// local project
#include "FilesystemSignalPosix.hpp"

namespace FSDB {
namespace filesystem {

Signal::Signal() {
  fd = inotify_init();
  if (fd < 0) {
    initError = true;
  } else {
    threadRunFlag = true;
    /* Just one thread watching for the fd = file descriptor
     */
    std::shared_ptr<std::thread> watcherThread =
        std::make_shared<std::thread>(&Signal::watch, this);
    watcherThread->detach();
  }
}

Signal::~Signal() {
  if (!initError) {
    threadRunFlag = false;
    for (auto wd : wdMap) {
      inotify_rm_watch(fd, wd.first);
    }
    close(fd);
  }
}

void Signal::watch() {
  /* What will happen when the instance is deleted?
   */
  while (threadRunFlag) {
    /* INOTIFY_EVENT_SIZE = 16
     */
    int length, i = 0;
    char buffer[INOTIFY_EVENT_BUF_LEN];

    // blocks until there is something to read
    length = read(fd, buffer, INOTIFY_EVENT_BUF_LEN);

    /* checking for error
     */
    if (length >= 0 && threadRunFlag) {
#if FILESYSTEM_SIGNAL_DEBUG
      std::cout << "Signal::watch(" << fd << ") length=" << length
                << " struct size=" << INOTIFY_EVENT_SIZE << "\n";
#endif
      /* actually read return the list of change events happens. Here, read the
       * change event one by one and process it accordingly.
       */
      while (i < length) {
        struct inotify_event *event = (struct inotify_event *)&buffer[i];
        boost::filesystem::path watchPath(wdMap.at(event->wd));
        boost::filesystem::path eventPath(std::string(event->name, event->len));
        boost::filesystem::path eventFullPath(watchPath / eventPath);
        std::unique_ptr<SignalEvent> sigEvent = std::make_unique<SignalEvent>();
        sigEvent->path = eventFullPath.generic_wstring();
#if FILESYSTEM_SIGNAL_DEBUG
        std::cout << "Signal::watch(" << fd << ") path=" << eventFullPath.generic_string() << "\n";
#endif
        if (event->mask & IN_CREATE) {
          if (event->mask & IN_ISDIR) {
            sigEvent->type = signalEventType::dirCreate;
          } else {
            sigEvent->type = signalEventType::fileCreate;
          }
        } else if (event->mask & IN_DELETE) {
          if (event->mask & IN_ISDIR) {
            sigEvent->type = signalEventType::dirDelete;
          } else {
            sigEvent->type = signalEventType::fileDelete;
          }
        }
        // signal
        (*sig)(std::move(sigEvent));
#if FILESYSTEM_SIGNAL_DEBUG
        std::cout << "Signal::watch(" << fd << ") len=" << event->len << "\n";
#endif
        i += INOTIFY_EVENT_SIZE + event->len;
      }
    } else {
      // error
    }
  }
}

bool Signal::addWatch(std::wstring pathNameWstr) {
  if (initError)
    return false;
  /* Only watches one directory deep
   */
  int wd =
      inotify_add_watch(fd, boost::filesystem::path(pathNameWstr).string().c_str(), IN_ALL_EVENTS);
  wdMap.insert({wd, boost::filesystem::path(pathNameWstr)});
  return true;
}

} // namespace filesystem
} // namespace FSDB

#endif // FSDB_FILESYSTEM_SIGNAL_H
