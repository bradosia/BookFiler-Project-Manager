/*
 * @name Filesystem Database Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief filesystem database and utilities
 */

/* rapidjson v1.1 (2016-8-25)
 * Developed by Tencent
 * License: MITs
 */
#include <rapidjson/prettywriter.h>

// Local Project
#include "Module.hpp"

/*
 * FSDB
 */
namespace FSDB {

ModuleExport::ModuleExport() {
  fileTable = std::make_shared<filesystem::FileTable>();
  fileTable->open();
  fileTable->createTable();
}

void ModuleExport::registerSettings(
    std::shared_ptr<rapidjson::Document> moduleRequest,
    std::shared_ptr<std::unordered_map<
        std::string, std::function<void(std::shared_ptr<rapidjson::Document>)>>>
        moduleCallbackMap) {
  moduleRequest->SetObject();
  moduleRequest->AddMember("FilesystemDatabase", "FilesystemDatabaseCB",
                           moduleRequest->GetAllocator());
  moduleCallbackMap->insert(
      {"FilesystemDatabaseCB",
       std::bind(&ModuleExport::setSettings, this, std::placeholders::_1)});
}

void ModuleExport::setSettings(std::shared_ptr<rapidjson::Value> data) {
#if MODULE_EXPORT_SET_SETTINGS_DEBUG
  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
  data->Accept(writer);
  std::cout << "FSDB::ModuleExport::setSettings:\n"
            << buffer.GetString() << std::endl;
#endif
  if (data->IsObject()) {
    // setDatabaseFileName
    auto result = data->FindMember("file_database_file_name");
    if (result != data->MemberEnd()) {
      if (result->value.IsString()) {
        fileTable->setDatabaseFileName(result->value.GetString());
      }
    }
    // insertDatabaseByJson
    auto result2 = data->FindMember("insert_database");
    if (result2 != data->MemberEnd()) {
      fileTable->insertDatabaseByJson(result2->value);
    }
    //
    auto result3 = data->FindMember("watch_directories");
    if (result3 != data->MemberEnd()) {
      addTestSignals(result3->value);
    }
  }
}

void ModuleExport::getDirectorySlot(
    std::string pathStr,
    std::function<void(std::shared_ptr<filesystem::FileTableData>)> callback) {
  std::cout << "FSDB::ModuleExport::getDirectorySlot(" << pathStr << ")\n";
  fileTable->getData(pathStr, callback);
}

int ModuleExport::newModel() {
  fileModelMap.insert(
      {fileModelMapIncrementId, std::make_shared<filesystem::FileModel>()});
  return fileModelMapIncrementId++;
}

int ModuleExport::newSignal() {
  signalMap.insert({signalIncrementId, std::make_shared<filesystem::Signal>()});
  return signalIncrementId++;
}

bool ModuleExport::connectSignal(
    int signalId, std::function<void(std::shared_ptr<filesystem::SignalEvent>)>
                      signalCallback) {
  auto found = signalMap.find(signalId);
  if (found == signalMap.end()) {
    return false;
  }
  found->second->connect(signalCallback);
  return true;
}
bool ModuleExport::watchSignal(int signalId, std::wstring pathWstr) {
  auto found = signalMap.find(signalId);
  if (found == signalMap.end()) {
    return false;
  }
  return found->second->addWatch(pathWstr);
}
bool ModuleExport::watchSignal(int signalId, std::string pathStr) {
  return watchSignal(
      signalId,
      std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(pathStr));
}

void ModuleExport::addTestSignals(rapidjson::Value &data) {
  if (data.IsArray()) {
    // rapidjson uses SizeType instead of size_t.
    rapidjson::SizeType n = data.Size();
    // iterate children
    for (rapidjson::SizeType i = 0; i < n; i++) {
      if (data[i].IsString()) {
        std::shared_ptr<filesystem::Signal> testSignal =
            std::make_shared<filesystem::Signal>();
        testSignal->addWatch(filesystem::toUTF16Wstring(data[i].GetString()));
        testSignal->connect(std::bind(&ModuleExport::testSignalCallback, this,
                                      std::placeholders::_1));
        testSignalMap.insert({testSignalIncrementId, testSignal});
        testSignalIncrementId++;
      }
    }
  }
}

void ModuleExport::testSignalCallback(
    std::shared_ptr<FSDB::filesystem::SignalEvent> signalEventPtr) {
  std::cout << "filesystem signal: path=\""
            << filesystem::toUTF8String(signalEventPtr->path)
            << "\" type=" << (int)signalEventPtr->type << "\n";
}

} // namespace FSDB
