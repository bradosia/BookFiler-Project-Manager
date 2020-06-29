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

#ifndef UCDPWAB_INTERFACE_H
#define UCDPWAB_INTERFACE_H

// c++17
#include <functional>
#include <memory>
#include <unordered_map>

/* rapidjson v1.1 (2016-8-25)
 * Developed by Tencent
 * License: MITs
 */
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>

/* QT 5.13.2-1
 * License: LGPLv3
 */
#include <QWidget>

/*
 * UCDPWAB = UC Davis PI Web API Browser
 */
namespace UCDPWAB {

class PluginInterface {
public:
  virtual void init() = 0;
  virtual std::shared_ptr<QWidget> getWidget() = 0;
  virtual void registerSettings(
      rapidjson::Document &pluginRequest,
      std::unordered_map<std::string, std::function<void(rapidjson::Value &)>>
          &pluginCallbackMap) = 0;
  /* Set the tree with indented text
   */
  virtual void treeSetPlainText(std::string &data) = 0;
  /* Set the tree with JSON
   */
  virtual void treeSetJSON(rapidjson::Value &data) = 0;
  /* load UC Davis building information
   * Does not display the data
   * @return Returns a progress handle
   */
  virtual int loadBuildingInfo(std::function<void(void)>&) = 0;
  virtual void viewBuildingInfo() = 0;
  virtual void loadProgress(int handle) = 0;
};

} // namespace UCDPWAB

#endif
// end UCDPWAB_INTERFACE_H
