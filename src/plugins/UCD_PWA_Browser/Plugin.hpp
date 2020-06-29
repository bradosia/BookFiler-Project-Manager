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

#ifndef UCDPWAB_PLUGIN_H
#define UCDPWAB_PLUGIN_H

// Universal Include
#include "UI/universalInclude.hpp"

#include "Interface.hpp"
#include "UI/MainWidget.hpp"
#include "UI/TreeItem.hpp"
#include "UI/TreeModel.hpp"
#include "core/HTTP_JSON.hpp"
#include "core/PiWebAPI.hpp"

/*
 * UCDPWAB = UC Davis PI Web API Browser
 */
namespace UCDPWAB {

/*
 * Plugin uses the MVC design pattern.
 * This is the controller, the view is a QT widget, and the model is the API
 * storage
 */
class PluginController : public PluginInterface {
private:
  std::shared_ptr<TreeModel> treeModel;
  std::shared_ptr<QTreeView> treeView;
  std::shared_ptr<PiWebAPI> api;

public:
  PluginController(QWidget *parent = nullptr) {
    treeModel = std::make_shared<TreeModel>();
    treeView = std::make_shared<QTreeView>();
    treeView->setModel(treeModel.get());
    api = std::make_shared<PiWebAPI>();
  }
  ~PluginController(){};

  void init() { printf("GREETINGS FROM DLL\n"); }
  std::shared_ptr<QWidget> getWidget() {
    std::shared_ptr<QWidget> widget = treeView;
    return widget;
  }
  void treeSetPlainText(std::string &data);
  void treeSetJSON(rapidjson::Value &data);
  void registerSettings(
      rapidjson::Document &pluginRequest,
      std::unordered_map<std::string, std::function<void(rapidjson::Value &)>>
          &pluginCallbackMap);
  int loadBuildingInfo(std::function<void(void)>& callback);
  void viewBuildingInfo();
  void loadProgress(int handle);
};

// Exporting `my_namespace::plugin` variable with alias name `plugin`
// (Has the same effect as `BOOST_DLL_ALIAS(my_namespace::plugin, plugin)`)
extern "C" BOOST_SYMBOL_EXPORT PluginController plugin;
PluginController plugin;

} // namespace UCDPWAB

#endif
// end UCDPWAB_PLUGIN_H
