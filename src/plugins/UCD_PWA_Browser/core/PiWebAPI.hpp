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

#ifndef UCDPWAB_PI_WEB_API_H
#define UCDPWAB_PI_WEB_API_H

// Universal Include
#include "UI/universalInclude.hpp"

// local project
#include "HTTP_JSON.hpp"

/*
 * UCDPWAB = UC Davis PI Web API Browser
 */
namespace UCDPWAB {

/* Might be used as building info elements in the future
 */
class BuildingInfo {};

class PiWebAPI {
private:
  bradosia::HTTP_JSON HTTP_JSON_Obj;
  std::shared_ptr<rapidjson::Document> buildingData;

public:
  PiWebAPI() {
      buildingData = std::make_shared<rapidjson::Document>();
  }
  ~PiWebAPI() {}
  int loadBuildingInfo(std::function<void(void)> &callback);
  void loadBuildingInfoCallback(size_t pos);
  void loadHTTP_Calls();
};

} // namespace UCDPWAB

#endif // UCDPWAB_PI_WEB_API_H
