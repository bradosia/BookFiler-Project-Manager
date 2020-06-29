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

#include "Application.hpp"

Application::Application(int &argc, char **argv) : QApplication(argc, argv) {
  mainWindow = std::make_unique<MainWindow>();
  mainWindow->show();
}

Application::~Application() {}
