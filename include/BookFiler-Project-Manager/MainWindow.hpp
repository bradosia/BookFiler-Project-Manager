/*
 * @name BookFiler Recognize
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief Text recognition viewer, editor, and analyzer.

 * Icons and images owned by their respective owners
 */

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

// Config
#include "../core/config.hpp"

// C++
#include <memory>

// bradosia libraries 1.0
#include <ModuleManager/ModuleManager.hpp>
#include <SettingsManager/SettingsManager.hpp>

// Modules
#include <BookFiler-Module-File-Tree-Pane/Interface.hpp>
#include <BookFiler-Module-Filesystem-Database/Interface.hpp>

// Local Project
#include "RenderWidget.hpp"
#include "ui_main.hpp"

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow() {
    for (auto widgetPtr : centralQWidgetPtrs) {
      widgetPtr->setParent(nullptr);
    }
  };
  void loadModules();
  int FSDB_ModuleLoaded(
      std::shared_ptr<bookfiler::FileSystemDatabaseInterface>);
  int fileTreePaneModuleLoaded(
      std::shared_ptr<bookfiler::FileTreePaneInterface>);
  int allModulesLoaded();

private:
  unsigned int modulesLoadedNum;
  unsigned int modulesLoadedTotalNum;
  std::unique_ptr<Ui::main> ui;
  std::shared_ptr<bookfiler::FileSystemDatabaseInterface> FSDB_Module;
  std::shared_ptr<bookfiler::FileTreePaneInterface> fileTreePaneModule;
  std::vector<std::shared_ptr<QWidget>> centralQWidgetPtrs;
  std::shared_ptr<bradosia::ModuleManager> moduleManagerPtr;
  std::shared_ptr<bradosia::SettingsManager> settingsManagerPtr;
  std::shared_ptr<bookfiler::FileTreePaneWidget> fileTreePaneWidgetList,
      fileTreePaneWidgetMain;
  std::shared_ptr<RenderWidget> renderWidgetList, renderWidgetMain;
};
#endif // MAIN_WINDOW_H
