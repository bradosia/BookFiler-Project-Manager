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

#ifndef UNIVERSAL_INCLUDE_H
#define UNIVERSAL_INCLUDE_H

// config
#include "../core/config.hpp"

// c++17
#include <algorithm> // std::find
#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

// c
#define _USE_MATH_DEFINES // M_PI
#include <cmath>
#include <stdio.h>
#include <stdlib.h> /* srand, rand */
#include <string.h>

/* QT 5.13.2-1
 * License: LGPLv3
 */
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QRubberBand>
#include <QString>
#include <QStyleFactory>
#include <QToolBar>
#include <QTranslator>
#include <QTreeView>
#include <QHBoxLayout>
#include <QObject>

#endif // UNIVERSAL_INCLUDE_H
