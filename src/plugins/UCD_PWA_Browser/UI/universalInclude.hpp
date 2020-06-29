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

#ifndef UCDPWAB_UNIVERSAL_INCLUDE_H
#define UCDPWAB_UNIVERSAL_INCLUDE_H

// config
#include "../core/config.hpp"

// c++17
//#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>

// c
#include <stdio.h>
#include <stdlib.h> /* srand, rand */
#include <string.h>

/* Boost
 */
#include <boost/config.hpp> // for BOOST_SYMBOL_EXPORT

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

/* QT 5.13.2-1
 * License: LGPLv3
 */
#include <QString>
#include <QTreeView>

#endif // UCDPWAB_UNIVERSAL_INCLUDE_H
