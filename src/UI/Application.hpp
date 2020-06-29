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

#ifndef APPLICATION_H
#define APPLICATION_H

// c++
#include <memory>

// QT
#include <QApplication>

// MainWindow
#include "MainWindow.hpp"

class Application final : public QApplication {
public:
  Application(int &argc, char **argv);
  ~Application();

private:
  std::unique_ptr<MainWindow> mainWindow;
};

#endif // APPLICATION_H
