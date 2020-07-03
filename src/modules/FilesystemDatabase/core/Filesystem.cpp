/*
 * @name Filesystem Database Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief filesystem database and utilities
 */

// local project
#include "Filesystem.hpp"

namespace FSDB {
namespace filesystem {

void traverse(
    boost::filesystem::path pathInitial,
    std::shared_ptr<std::unordered_map<std::wstring, bool>> visitedPtr) {
  std::queue<boost::filesystem::path> directoryQueue;
  directoryQueue.push(pathInitial);
  while (!directoryQueue.empty()) {
    try {
      /* As of 2020-03-30
       * Highest version of boost on ubuntu is 1.65.1.0ubuntu1
       * Highest version of QT on ubuntu is 5.9.5+dfsg-0ubuntu2.5
       * boost::filesystem::directory_options::skip_permission_denied is for
       * boost 1.72 so we must wait for ubuntu to update
       */
      for (auto &p :
           boost::filesystem::directory_iterator(directoryQueue.front())) {
        if (boost::filesystem::is_directory(p)) {
          directoryQueue.push(p);
        }
        visitedPtr->insert({p.path().wstring(), false});
#if TRAVERSE_DEBUG
        std::cout << p.path() << "\n";
#endif
      }
    } catch (const std::exception &e) {
#if TRAVERSE_DEBUG
      std::cout << e.what() << "\n";
#endif
    }
    directoryQueue.pop();
  }
}

std::string toUTF8String(std::wstring wstr) {
  return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wstr);
}
std::wstring toUTF16Wstring(std::string str) {
  return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
}

} // namespace filesystem
} // namespace FSDB
