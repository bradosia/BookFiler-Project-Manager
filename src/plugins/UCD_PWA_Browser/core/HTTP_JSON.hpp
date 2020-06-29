/*
 * @name HTTP JSON
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief Manages HTTP JSON Calls
 */

#ifndef BRADOSIA_HTTP_JSON_H
#define BRADOSIA_HTTP_JSON_H

// C++
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>

/* rapidjson v1.1 (2016-8-25)
 * Developed by Tencent
 * License: MITs
 */
#include <rapidjson/document.h>

/*
 * CURL 7.68.0
 * License: MIT/X inspired
 */
#define CURL_STATICLIB
#include <brotli/decode.h>
#include <curl/curl.h>

namespace bradosia {

/* According to:
 * https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
 * This method is more performant than string stream and streambuf iterator
 */
std::string readFile(std::fstream &fileStream);

/*
 * Makes HTTP and HTTPS calls to a JSON API.
 */
class HTTP_JSON {
private:
  std::shared_ptr<rapidjson::Document> API_Document;

public:
  std::vector<std::shared_ptr<rapidjson::Document>> documents;

  HTTP_JSON() { API_Document = std::make_shared<rapidjson::Document>(); }
  ~HTTP_JSON() {}
  /* Makes an HTTPS GET request to the URI
   * @param URI The address
   */
  bool get(std::string URI, size_t *pos);
  /* CALLBACK - Makes an HTTPS GET request to the URI
   * @param URI The address
   */
  void getCallback(std::string URI, std::function<void(size_t)> callback);
  /* THREADED - Makes an HTTPS GET request to the URI
   * @param URI The address
   */
  void getThreaded(std::string URI, size_t &pos);
  /* THREADED + CALLBACK - Makes an HTTPS GET request to the URI
   * @param URI The address
   */
  void getThreadedCallback(std::string URI,
                           std::function<void(size_t)> callback);
  void loadHTTP_Calls(std::string fileName);
  std::weak_ptr<rapidjson::Document> getAPI_Document();
};

/* Writes data from HTTP request into a string buffer
 * @param ptr data address
 */
size_t writefunc(void *ptr, size_t size, size_t nmemb, std::string *s);

} // namespace bradosia

#endif // BRADOSIA_HTTP_JSON_H
