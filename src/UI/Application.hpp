/*
 * @name BookFiler Recognize
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief Text recognition viewer, editor, and analyzer.

 * Icons and images owned by their respective owners
 */

#ifndef APPLICATION_H
#define APPLICATION_H

// c++
#include <memory>

/* QT 5.13.2-1
 * License: LGPLv3
 */
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
