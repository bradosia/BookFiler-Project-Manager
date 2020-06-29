/*
 * @name HTTP JSON
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief Manages HTTP JSON Calls
 */

// local project
#include "HTTP_JSON.hpp"

namespace bradosia {

std::string readFile(std::fstream &fileStream) {
  fileStream.seekg(0, std::ios::end);
  size_t size = fileStream.tellg();
  std::string contentStr(size, ' ');
  fileStream.seekg(0);
  fileStream.read(&contentStr[0], size);
  return contentStr;
}

bool HTTP_JSON::get(std::string URI, size_t *pos) {
  CURL *curl;
  CURLcode res;
  std::string bufferString;
  std::shared_ptr<rapidjson::Document> resJSON_Doc =
      std::make_shared<rapidjson::Document>();

  printf("HTTPS_GET_JSON called on: %s\n", URI.c_str());

  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();

  if (curl) {

    curl_easy_setopt(curl, CURLOPT_URL, URI.c_str());
    // curl_easy_setopt(curl, CURLOPT_CAINFO,
    // "/etc/ssl/certs/ca-certificates.crt");

    /*
     * TODO: Write the SSL Certificate Manager
     */
    // getCertificates(global);
#ifdef SKIP_PEER_VERIFICATION
    /*
     * If you want to connect to a site who isn't using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
    /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

    /* setup callbacks */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &bufferString);
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    resJSON_Doc->Parse(bufferString.c_str());
    pos = new size_t(documents.size()); // insert pos
    documents.push_back(resJSON_Doc);

    /* Check for errors */
    if (res == CURLE_OK) {
      return true;
    } else {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }

#ifdef DEBUG_JSON
    std::cout << bufferString << std::endl;
#endif
    /* always cleanup */
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();
  return false;
}

void HTTP_JSON::getCallback(std::string URI,
                            std::function<void(size_t)> callback) {
  size_t *pos;
  get(URI, pos);
  if (callback) {
    callback(*pos);
  }
}

void HTTP_JSON::getThreaded(std::string URI, size_t &pos) {
  size_t *posa;
  std::thread thread1(&HTTP_JSON::get, this, "URI", posa);
  thread1.detach();
}

void HTTP_JSON::getThreadedCallback(std::string URI,
                                    std::function<void(size_t)> callback) {
  std::thread thread1(&HTTP_JSON::getCallback, this, URI, callback);
  thread1.detach();
}

void HTTP_JSON::loadHTTP_Calls(std::string fileName) {
  // Check if settings file exists
  {
    std::fstream inputFile;
    inputFile.open(fileName, std::fstream::in);
    if (!inputFile) {
      printf("FATAL ERROR: Could not open input file: %s\n", fileName.c_str());
      return;
    }
  }
  // open settings file
  std::fstream settingsFile;
  printf("Opening settings file: %s\n", fileName.c_str());
  settingsFile.open(fileName, std::fstream::in);
  if (settingsFile) {
    std::string contentStr = readFile(settingsFile);
#if HTTP_JSON_DEBUG
    std::cout << contentStr << std::endl;
#endif
    rapidjson::Document d;
    d.Parse<rapidjson::kParseCommentsFlag |
            rapidjson::kParseTrailingCommasFlag>(contentStr.c_str());
    if (d.IsObject()) {
      std::cout << "SETTINGS deployFile(" << fileName << ") deployDOM()"
                << std::endl;
      // merge plugin settings with master
      for (rapidjson::Value::MemberIterator itr = d.MemberBegin();
           itr != d.MemberEnd(); ++itr) {
        API_Document->AddMember(itr->name, itr->value,
                                API_Document->GetAllocator());
      }
    } else {
      std::cout << "SETTINGS deployFile(" << fileName << ") Parse() FAILED!"
                << std::endl;
    }
  } else {
    printf("No settings file used. Check settings file: %s\n",
           fileName.c_str());
  }
}

std::weak_ptr<rapidjson::Document> HTTP_JSON::getAPI_Document() {
  return API_Document;
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, std::string *s) {
  s->append((char *)ptr, size * nmemb);
  return size * nmemb;
}

} // namespace bradosia
