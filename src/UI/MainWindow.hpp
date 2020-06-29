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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

// main UI
#include "ui_main.h"

// universal includes
#include "universalInclude.hpp"

// UCD_PWA_Browser
#include <UCD_PWA_Browser/Interface.hpp>

#include "PluginManager/PluginManager.hpp"
#include "SettingsManager/SettingsManager.hpp"

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow(){
      for (auto widgetPtr : centralQWidgetPtrs) {
        widgetPtr->setParent(nullptr);
      }
  };

private:
  // ui
  std::unique_ptr<Ui::main> ui;
  std::shared_ptr<UCDPWAB::PluginInterface> UCDPWAB_plugin;
  std::vector<std::shared_ptr<QWidget>> centralQWidgetPtrs;

private slots:
  void on_actionEnergy_triggered();

  void on_actionDiscover_triggered();

  void on_actionWater_triggered();

  void on_actionWiFi_triggered();

  void on_actionTemperature_triggered();
};
#endif // MAIN_WINDOW_H
