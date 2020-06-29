/*
 * @name UC Davis PI Web API Browser
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief Browser for UC Davis PI Web API data.
 *
 * Data from OSIsoft and UC Davis
 * Icons and images owned by their respective owners
 */

/*
 * piwebapi-ucdavis 1.0
 */
#include "Plugin.hpp"

/*
 * UCDPWAB = UC Davis PI Web API Browser
 */
namespace UCDPWAB {

void PluginController::treeSetPlainText(std::string &data) {
  treeModel->addData(data);
}

void PluginController::treeSetJSON(rapidjson::Value &data) {
#if PLUGIN_DEBUG
  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
  data.Accept(writer);
  std::cout << "treeSetJSON() data:\n" << buffer.GetString() << std::endl;
#endif
  treeModel->addData(data);
  treeView->update();
}

void PluginController::registerSettings(
    rapidjson::Document &pluginRequest,
    std::unordered_map<std::string, std::function<void(rapidjson::Value &)>>
        &pluginCallbackMap) {
  pluginRequest.SetObject();
  pluginRequest.AddMember("treeData", "treeSet", pluginRequest.GetAllocator());
  pluginCallbackMap.insert({"treeSet", std::bind(&PluginController::treeSetJSON,
                                                 this, std::placeholders::_1)});
#if PLUGIN_DEBUG
  std::cout << "Plugin Callback Map:\n";
  for (auto pairs : pluginCallbackMap) {
    std::cout << "first:" << pairs.first << std::endl;
  }
#endif
}

int PluginController::loadBuildingInfo(std::function<void(void)> &callback) {
  api->loadBuildingInfo(callback);
  return 0;
}
void PluginController::viewBuildingInfo() {

}
void PluginController::loadProgress(int handle) {

}

} // namespace UCDPWAB
