/*
 * @name Settings Manager
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief Manages application settings
 */

//
#include <rapidjson/error/en.h>

// Local Project
#include "SettingsManager.hpp"

namespace bradosia {

std::string readFile(std::fstream &fileStream) {
  fileStream.seekg(0, std::ios::end);
  size_t size = fileStream.tellg();
  std::string contentStr(size, ' ');
  fileStream.seekg(0);
  fileStream.read(&contentStr[0], size);
  return contentStr;
}

bool SettingsManager::deployFile(std::string settingsFileString) {
  // Check if settings file exists
  {
    std::fstream inputFile;
    inputFile.open(settingsFileString, std::fstream::in);
    if (!inputFile) {
#if SETTINGS_MANAGER_VERBOSE
      printf("FATAL ERROR: Could not open input file: %s\n",
             settingsFileString.c_str());
#endif
      return false;
    }
  }
  // open settings file
  std::fstream settingsFile;
#if SETTINGS_MANAGER_VERBOSE
  std::cout << "Opening settings file: " << settingsFileString << std::endl;
#endif
  settingsFile.open(settingsFileString, std::fstream::in);
  if (settingsFile) {
    std::string contentStr = readFile(settingsFile);
#if SETTINGS_MANAGER_DEPLOY_FILE_ECHO_DEBUG
    std::cout << contentStr << std::endl;
#endif
    std::shared_ptr<rapidjson::Document> d =
        std::make_shared<rapidjson::Document>();
    /* rapidjson::kParseStopWhenDoneFlag used to prevent strange
     * errors occuring on valid JSON files.
     * TODO: Why are there errors on valid JSON?
     * (The JSON was checked with another tool, but failed here)
     */
    if (d->Parse<rapidjson::kParseCommentsFlag |
                 rapidjson::kParseTrailingCommasFlag |
                 rapidjson::kParseStopWhenDoneFlag>(contentStr.c_str())
            .HasParseError()) {
#if SETTINGS_MANAGER_DEPLOY_FILE_DEBUG
      std::cout << "SETTINGS deployFile(" << settingsFileString
                << ") Parse() FAILED!" << std::endl;
      unsigned int start = 0;
      if (d->GetErrorOffset() - 50 >= 0) {
        start = d->GetErrorOffset() - 50;
      }
      unsigned int end = contentStr.length() - start;
      if ((d->GetErrorOffset() + 50) < contentStr.length()) {
        end = 50;
      }
      std::cout << "Error at offset=" << d->GetErrorOffset() << ", "
                << contentStr.substr(start, end) << "\n"
                << GetParseError_En(d->GetParseError()) << std::endl;
#endif
      return false;
    }
    std::cout << "SETTINGS deployFile(" << settingsFileString << ") deployDOM()"
              << std::endl;
    deploy(d, master);
  } else {
    printf("No settings file used. Check settings file: %s\n",
           settingsFileString.c_str());
  }
  return true;
}

bool SettingsManager::merge(
    std::shared_ptr<rapidjson::Value> module,
    std::shared_ptr<std::unordered_map<
        std::string, std::function<void(std::shared_ptr<rapidjson::Document>)>>>
        moduleCallbackMap) {
  // merge module settings with master
  if (module->IsObject()) {
    for (rapidjson::Value::MemberIterator itr = module->MemberBegin();
         itr != module->MemberEnd(); ++itr) {
      master->AddMember(itr->name, itr->value, master->GetAllocator());
    }
  }
#if SETTINGS_MANAGER_DEBUG
  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
  master->Accept(writer);
  std::cout << "Master:\n" << buffer.GetString() << std::endl;
#endif
  // merge module callbacks with master
  masterCallbackMap.insert(moduleCallbackMap->begin(),
                           moduleCallbackMap->end());
#if SETTINGS_MANAGER_DEBUG
  std::cout << "module Callback Map:\n";
  for (auto pairs : *moduleCallbackMap) {
    std::cout << "first:" << pairs.first << std::endl;
  }
  std::cout << "Master Callback Map:\n";
  for (auto pairs : masterCallbackMap) {
    std::cout << "first:" << pairs.first << std::endl;
  }
#endif
  return true;
}

bool SettingsManager::deploy(std::shared_ptr<rapidjson::Value> settingsPtr,
                             std::shared_ptr<rapidjson::Value> masterPtr) {
  std::queue<std::pair<rapidjson::Value *, rapidjson::Value *>> valueQueue;
  valueQueue.push({settingsPtr.get(), masterPtr.get()});
  while (!valueQueue.empty()) {
    // Raw pointers because its faster in a loop
    rapidjson::Value *settingsValuePtr = valueQueue.front().first;
    rapidjson::Value *masterValuePtr = valueQueue.front().second;
    valueQueue.pop();
    for (auto masterIt = masterValuePtr->MemberBegin();
         masterIt != masterValuePtr->MemberEnd(); ++masterIt) {
#if SETTINGS_MANAGER_DEPLOY_DEBUG
      /* BUG:
       * This line makes a segmentation fault
       * when masterObject has nested objects
       */
      std::string masterObjectName = masterIt->name.GetString();
      std::cout << "SETTINGS deployFile() deploy() find(" << masterObjectName
                << ")" << std::endl;
#endif
      /* Find if settings has same name as master
       */
      auto dstIt = settingsValuePtr->FindMember(masterIt->name);
      if (dstIt != settingsValuePtr->MemberEnd()) {
#if SETTINGS_MANAGER_DEPLOY_DEBUG
        std::cout << "SETTINGS deployFile() deploy() find(" << masterObjectName
                  << ") FOUND!\n";
#endif

        if (masterIt->value.IsObject() && dstIt->value.IsObject()) {
#if SETTINGS_MANAGER_DEPLOY_DEBUG
          std::cout << "SETTINGS deployFile() deploy() recurse...\n";
#endif
          valueQueue.push({&(dstIt->value), &(masterIt->value)});
        } else if (masterIt->value.IsString()) {
          /* Names match, so now search for matching callback
           */
#if SETTINGS_MANAGER_DEPLOY_DEBUG
          std::cout << "SETTINGS deployFile() deploy() find callback...\n";
#endif
          // find callback
          std::string callbackHandle = masterIt->value.GetString();
          auto findIt = masterCallbackMap.find(callbackHandle);
          if (findIt != masterCallbackMap.end()) {
            // call callback
#if SETTINGS_MANAGER_DEPLOY_DEBUG
            std::cout << "SETTINGS deployFile() deploy() find("
                      << masterIt->name.GetString() << ") Callback("
                      << callbackHandle << ")" << std::endl;
#endif
            /* encapsulate the value in a new document
             * CopyFrom does a deep copy
             */
            std::shared_ptr<rapidjson::Document> d =
                std::make_shared<rapidjson::Document>();
            d->CopyFrom(dstIt->value, d->GetAllocator());
            (findIt->second)(d);
#if SETTINGS_MANAGER_DEPLOY_DEBUG
            std::cout << "SETTINGS deployFile() deploy() find("
                      << masterIt->name.GetString() << ") Callback("
                      << callbackHandle << ") finished" << std::endl;
#endif
          }
        }
      }
    }
  }
  return true;
}

} // namespace bradosia
