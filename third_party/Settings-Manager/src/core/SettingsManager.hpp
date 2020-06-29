/*
 * @name Settings Manager
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief Manages application settings
 */

#ifndef BRADOSIA_SETTINGS_MANAGER_H
#define BRADOSIA_SETTINGS_MANAGER_H

#define SETTINGS_MANAGER_DEBUG 0
#define SETTINGS_MANAGER_DEPLOY_FILE_ECHO_DEBUG 0
#define SETTINGS_MANAGER_DEPLOY_FILE_DEBUG 0
#define SETTINGS_MANAGER_DEPLOY_DEBUG 1
#define SETTINGS_MANAGER_VERBOSE 1

// C
#include <string.h> // memset

// C++
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>

/* rapidjson v1.1 (2016-8-25)
 * Developed by Tencent
 * License: MITs
 */
#include <rapidjson/document.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/reader.h> // rapidjson::ParseResult
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace bradosia {

/* According to:
 * https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
 * This method is more performant than string stream and streambuf iterator
 */
std::string readFile(std::fstream &fileStream);

/* This class is for merging settings from multiple modules
 * then deploying them back to the modules.
 *
 * Enables users to only need one settings file
 * with only applicable settings deployed to each module
 */
class SettingsManager {
  std::string settingsFileNameStr;
  // settings to look for
  std::shared_ptr<rapidjson::Document> master;
  std::unordered_map<std::string,
                     std::function<void(std::shared_ptr<rapidjson::Document>)>>
      masterCallbackMap;

public:
  SettingsManager() {
    master = std::make_shared<rapidjson::Document>();
    master->SetObject();
  };
  ~SettingsManager(){};
  bool deployFile(std::string settingsFileString);

  /* Merges the module settings with the master
   */
  bool merge(std::shared_ptr<rapidjson::Value> data,
             std::shared_ptr<std::unordered_map<
                 std::string,
                 std::function<void(std::shared_ptr<rapidjson::Document>)>>>
                 moduleCallbackMap);
  /* Recursive deploying of settings callbacks
   * Objects discovered with registered names
   * will de deployed with the registered callback
   * named callbacks registered with merge()
   */
  bool deploy(std::shared_ptr<rapidjson::Value> dstObject,
              std::shared_ptr<rapidjson::Value> srcObject);
};

} // namespace bradosia

#endif
// BRADOSIA_SETTINGS_MANAGER_H
