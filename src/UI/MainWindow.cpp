/*
 * @name BookFiler Recognize
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief Text recognition viewer, editor, and analyzer.

 * Icons and images owned by their respective owners
 */

// C++
#include <functional>
#include <thread>

// Local Project
#include "MainWindow.hpp"

int callbackTest(
    std::shared_ptr<bookfiler::FileSystemDatabaseInterface> module_) {
  std::cout << "---------------CALLBACK TEST\n";
  return 0;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  /* UI Setup
   */
  ui = std::make_unique<Ui::main>();
  ui->setupUi(this);
  this->setObjectName("MainWindow");

  /* Load modules in another thread and let them appear as they load.
   * This will give the illusion to the user that the load time is fast.
   * A splash screen or loading bar will make the program start up seem slow.
   *
   * EDIT: can't do this because QT gives error:
   * QObject: Cannot create children for a parent that is in a different thread.
   */
  modulesLoadedNum = 0;
  modulesLoadedTotalNum = 6;
  // std::thread loadModulesThread(&MainWindow::loadModules, this);
  // loadModulesThread.join();
  loadModules();
}

void MainWindow::loadModules() {
#if MAIN_WINDOW_DEBUG
  std::cout << "MainWindow::loadModules() BEGIN\n";
#endif
  settingsManagerPtr = std::make_shared<bradosia::SettingsManager>();
  /* Module Load
   */
  moduleManagerPtr = std::make_shared<bradosia::ModuleManager>();
  moduleManagerPtr->addModule<bookfiler::FileSystemDatabaseInterface>(
      "bookfilerFilesystemDatabaseModule");
  moduleManagerPtr
      ->getCallbackLoadSignal<bookfiler::FileSystemDatabaseInterface>(
          "bookfilerFilesystemDatabaseModule")
      ->connect(std::bind(&MainWindow::FSDB_ModuleLoaded, this,
                          std::placeholders::_1));
  moduleManagerPtr->addModule<bookfiler::FileTreePaneInterface>(
      "bookfilerFileTreePaneModule");
  moduleManagerPtr
      ->getCallbackLoadSignal<bookfiler::FileTreePaneInterface>(
          "bookfilerFileTreePaneModule")
      ->connect(std::bind(&MainWindow::fileTreePaneModuleLoaded, this,
                          std::placeholders::_1));
  moduleManagerPtr->callbackLoadAllSignal.connect(
      std::bind(&MainWindow::allModulesLoaded, this));
  moduleManagerPtr->loadModules("modules");
#if MAIN_WINDOW_DEBUG
  std::cout << "MainWindow::loadModules() END\n";
#endif
}

int MainWindow::FSDB_ModuleLoaded(
    std::shared_ptr<bookfiler::FileSystemDatabaseInterface> module_) {
  std::cout << "MainWindow::FSDB_ModuleLoaded()" << std::endl;
  FSDB_Module = module_;
  return 0;
}

int MainWindow::fileTreePaneModuleLoaded(
    std::shared_ptr<bookfiler::FileTreePaneInterface> module) {
  std::cout << "MainWindow::fileTreePaneModuleLoaded()" << std::endl;
  fileTreePaneModule = module;
  /* register widgets
   */
  fileTreePaneModule->init();
  /* register setting deploy
   */
  std::shared_ptr<rapidjson::Document> moduleRequest =
      std::make_shared<rapidjson::Document>();
  std::shared_ptr<std::unordered_map<
      std::string, std::function<void(std::shared_ptr<rapidjson::Document>)>>>
      moduleCallbackMap = std::make_shared<std::unordered_map<
          std::string,
          std::function<void(std::shared_ptr<rapidjson::Document>)>>>();
  fileTreePaneModule->registerSettings(moduleRequest, moduleCallbackMap);
  settingsManagerPtr->merge(moduleRequest, moduleCallbackMap);
  /* get widget
   */
  std::cout << "hocrEditModule->getWidget()" << std::endl;
  fileTreePaneWidgetList = fileTreePaneModule->newWidget();
  fileTreePaneWidgetMain = fileTreePaneModule->newWidget();
  std::cout << "std::make_shared<RenderWidget>()" << std::endl;
  renderWidgetList = std::make_shared<RenderWidget>();
  renderWidgetMain = std::make_shared<RenderWidget>();
  renderWidgetList->renderFunction =
      std::bind(&bookfiler::FileTreePaneWidget::render, fileTreePaneWidgetList,
                std::placeholders::_1);
  renderWidgetList->initGraphicsFunction =
      std::bind(&bookfiler::FileTreePaneWidget::initGraphics,
                fileTreePaneWidgetList, std::placeholders::_1);
  renderWidgetMain->renderFunction =
      std::bind(&bookfiler::FileTreePaneWidget::render, fileTreePaneWidgetMain,
                std::placeholders::_1);
  renderWidgetMain->initGraphicsFunction =
      std::bind(&bookfiler::FileTreePaneWidget::initGraphics,
                fileTreePaneWidgetMain, std::placeholders::_1);
  // renderWidget->renderFunction();
  // needed to prevent crashing on program exit
  centralQWidgetPtrs.push_back(renderWidgetList);
  centralQWidgetPtrs.push_back(renderWidgetMain);
  // TODO: Check if widgets need to be added on paint cycles
  QSizePolicy policy = renderWidgetMain->sizePolicy();
  policy.setHorizontalStretch(2);
  renderWidgetMain->setSizePolicy(policy);
  ui->horizontalSplitter->addWidget(renderWidgetMain.get());
  return 0;
}

int MainWindow::allModulesLoaded() {
  /* Get the settings
   */
  settingsManagerPtr->deployFile(SETTINGS_FILE);
  /* SIGNALS AND SLOTS HOOKUP
   */
  // Ui::main must inherit QObject for slot to work.
  connect(ui->actionAbout, &QAction::triggered, ui.get(), &Ui::main::about);
  connect(ui->actionOpen, &QAction::triggered, ui.get(),
          &Ui::main::selectFiles);
  connect(ui->listWidget, &QListWidget::itemDoubleClicked, ui.get(),
          &Ui::main::listItemActivated);

  fileTreePaneWidgetList->viewTreeDataSlot;
  fileTreePaneWidgetList->viewTreeJsonSlot;
  // fileTreePaneWidgetList->getDirectorySignal;
  return 0;
}
