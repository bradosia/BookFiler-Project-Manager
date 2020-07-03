/*
 * @name BookFiler Recognize
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief Text recognition viewer, editor, and analyzer.

 * Icons and images owned by their respective owners
 */

#include "Application.hpp"

Application::Application(int &argc, char **argv) : QApplication(argc, argv) {
  addLibraryPath("");
  mainWindow = std::make_unique<MainWindow>();
  mainWindow->show();
}

Application::~Application() {}
