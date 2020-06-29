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

#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  /* UI Setup
   */
  ui = std::make_unique<Ui::main>();
  ui->setupUi(this);
  this->setObjectName("MainWindow");

  /* Plugins
   */
  bradosia::PluginManager pluginManagerObj;
  bradosia::SettingsManager settingsManagerObj;
  pluginManagerObj.addPluginInterface<UCDPWAB::PluginInterface>("plugin");
  pluginManagerObj.loadPlugins("plugins");
  UCDPWAB_plugin =
      pluginManagerObj.getPlugin<UCDPWAB::PluginInterface>("plugin");
  if (UCDPWAB_plugin) {
    printf("UCDPWAB PLUGIN FOUND\n");
    /* register widgets
     */
    UCDPWAB_plugin->init();
    std::shared_ptr<QWidget> UCDPWAB_Widget = UCDPWAB_plugin->getWidget();
    centralQWidgetPtrs.push_back(UCDPWAB_Widget);
    /* register settings
     */
    rapidjson::Document pluginRequest;
    std::unordered_map<std::string,
                       std::function<void(rapidjson::Value & data)>>
        pluginCallbackMap;
    UCDPWAB_plugin->registerSettings(pluginRequest, pluginCallbackMap);
    settingsManagerObj.merge(pluginRequest, pluginCallbackMap);

    // Plain text tree start
    // Open resource file
    QFile file(":menu/default.txt");
    file.open(QIODevice::ReadOnly);
    // Add to Tree
    // UCDPWAB_plugin->treeSetPlainText(file.readAll());
    // Close file
    file.close();
  }

  /* Get the settings
   */
  settingsManagerObj.deployFile(SETTINGS_FILE);

  /* find the layout in the centralWidget
   * add the plugin widgets to the layout
   */
  printf("===== DEBUG: ATTEMPTING TO FIND CENTRAL... =====\n");
  QList<QHBoxLayout *> widgetCentralWidgetList =
      this->findChildren<QHBoxLayout *>("horizontalLayout",
                                        Qt::FindChildrenRecursively);
  if (!widgetCentralWidgetList.empty()) {
    printf("===== DEBUG: CENTRAL FOUND =====\n");
    //QHBoxLayout* centralLayout = widgetCentralWidgetList.at(0);
    for (auto widgetPtr : centralQWidgetPtrs) {
      widgetCentralWidgetList.at(0)->addWidget(widgetPtr.get());
    }
  }

  // QTreeView specific
  if(!centralQWidgetPtrs.empty()){
      std::shared_ptr<QTreeView> test = std::dynamic_pointer_cast<QTreeView>(centralQWidgetPtrs[0]);
      test->expandAll();
      test->setColumnWidth(0, 200);
  }

  if (UCDPWAB_plugin) {
      //UCDPWAB_plugin->loadBuildingInfo();
  }
}

void MainWindow::on_actionEnergy_triggered() {
  /*std::string webID =
      "A0EbgZy4oKQ9kiBiZJTW7eugwC6-3Qzx_"
      "5RGrBZiQlqSuWw2sDVYNIPR1YODsG1RUyETgVVRJTC1BRlxDRUZTXFVDREFWSVNcQl"
      "VJTE"
      "RJTkdTXEFDQURFTUlDIFNVUkdFIEJVSUxESU5HXEVMRUNUUklDSVRZfERFTUFORA";
  std::filesystem::path streamHTTP =
      "https://ucd-pi-iis.ou.ad3.ucdavis.edu/piwebapi/streams/";
  std::filesystem::path electricityHTTP = streamHTTP / webID / "recorded";
  rapidjson::Document resJSON_Doc =
      PWA_UCD::HTTPS_GET_JSON(electricityHTTP.string());

  if (resJSON_Doc.IsObject()) {
    for (rapidjson::Value::ConstMemberIterator itr = resJSON_Doc.MemberBegin();
         itr != resJSON_Doc.MemberEnd(); ++itr) {
      PWA_UCD::printJSON_iterator(itr, 0);
    }
  }*/
}

void MainWindow::on_actionDiscover_triggered() {
  /*std::string HTTP_request =
      "https://ucd-pi-iis.ou.ad3.ucdavis.edu/piwebapi/dataservers/"
      "F1DS9KoOKByvc0-uxyvoTV1UfQVVRJTC1QSS1Q/points";
  rapidjson::Document resJSON_Doc = PWA_UCD::HTTPS_GET_JSON(HTTP_request);

  if (resJSON_Doc.IsObject()) {
    for (rapidjson::Value::ConstMemberIterator itr = resJSON_Doc.MemberBegin();
         itr != resJSON_Doc.MemberEnd(); ++itr) {
      PWA_UCD::printJSON_iterator(itr, 0);
    }
  }*/
}

void MainWindow::on_actionWater_triggered() {
  /*std::string webID =
      "F1DSAAAAAAAAAAAAAAAAEiCt5wVUNELVBJLUlJUy5PVS5BRDMuVUNEQVZJUy5FRFU";
  std::filesystem::path streamHTTP =
      "https://ucd-pi-iis.ou.ad3.ucdavis.edu/piwebapi/streams/";
  std::filesystem::path electricityHTTP = streamHTTP / webID / "recorded";
  rapidjson::Document resJSON_Doc =
      PWA_UCD::HTTPS_GET_JSON(electricityHTTP.string());

  if (resJSON_Doc.IsObject()) {
    for (rapidjson::Value::ConstMemberIterator itr = resJSON_Doc.MemberBegin();
         itr != resJSON_Doc.MemberEnd(); ++itr) {
      PWA_UCD::printJSON_iterator(itr, 0);
    }
  }*/
}

void MainWindow::on_actionWiFi_triggered() {
  /*
    std::string inputURIString, outputFileString;
  PWA_UCD::getSettingsFile(SETTINGS_FILE, inputURIString,outputFileString);
  rapidjson::Document resJSON_Doc =
      PWA_UCD::HTTPS_GET_JSON(inputURIString);
  std::ofstream ofs(outputFileString.c_str());
  if ( !ofs.is_open() )
      {
     printf("ERROR: output file not found: %s\n",outputFileString.c_str());
        return ;
      }

       rapidjson::OStreamWrapper osw { ofs };
       rapidjson::PrettyWriter< rapidjson::OStreamWrapper> writer2 { osw };
      resJSON_Doc.Accept( writer2 );
      */
}

void MainWindow::on_actionTemperature_triggered() {
  // Open resource file
  QFile file(":menu/default.txt");
  file.open(QIODevice::ReadOnly);
  // Add to Tree
  // UCD_PWA_Data_Widget->treeAddPlainText(file.readAll());
  // Close file
  file.close();
}
