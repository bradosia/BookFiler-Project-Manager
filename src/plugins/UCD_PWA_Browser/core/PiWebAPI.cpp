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

// local project
#include "PiWebAPI.hpp"

/*
 * UCDPWAB = UC Davis PI Web API Browser
 */
namespace UCDPWAB {

int PiWebAPI::loadBuildingInfo(std::function<void(void)> &callback) {
  std::weak_ptr<rapidjson::Document> API_Document =
      HTTP_JSON_Obj.getAPI_Document();
  std::shared_ptr<rapidjson::Document> API_DocumentSPtr = API_Document.lock();
  std::function<void(size_t)> cb = std::bind(
      &PiWebAPI::loadBuildingInfoCallback, this, std::placeholders::_1);
  auto element = API_DocumentSPtr->FindMember("building_info");
  if (element != API_DocumentSPtr->MemberEnd()) {
    HTTP_JSON_Obj.getThreadedCallback(element->value.GetString(), cb);
  }
}

void PiWebAPI::loadBuildingInfoCallback(size_t pos) {
  std::shared_ptr<rapidjson::Document> d = HTTP_JSON_Obj.documents.at(pos);
  if (d->IsObject()) {
    for (rapidjson::Value::MemberIterator itr = d->MemberBegin();
         itr != d->MemberEnd(); ++itr) {
      buildingData->AddMember(itr->name, itr->value,
                              buildingData->GetAllocator());
    }
  }
}

void PiWebAPI::loadHTTP_Calls() { HTTP_JSON_Obj.loadHTTP_Calls(API_FILE); }

} // namespace UCDPWAB
