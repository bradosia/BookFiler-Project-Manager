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
  moduleManagerPtr->addModule<bookfiler::FSDB_Interface>("bookfilerFSDBModule");
  moduleManagerPtr
      ->getCallbackLoadSignal<bookfiler::FSDB_Interface>("bookfilerFSDBModule")
      .connect(std::bind(&MainWindow::FSDB_ModuleLoaded, this,
                         std::placeholders::_1));
  moduleManagerPtr->addModule<bookfiler::FileTreePaneInterface>(
      "bookfilerFileTreePaneModule");
  moduleManagerPtr
      ->getCallbackLoadSignal<bookfiler::FileTreePaneInterface>(
          "bookfilerFileTreePaneModule")
      .connect(std::bind(&MainWindow::fileTreePaneModuleLoaded, this,
                         std::placeholders::_1));
  moduleManagerPtr->callbackLoadAllSignal.connect(
      std::bind(&MainWindow::allModulesLoaded, this));
  moduleManagerPtr->loadModules("modules");
#if MAIN_WINDOW_DEBUG
  std::cout << "MainWindow::loadModules() END\n";
#endif
}

void MainWindow::FSDB_ModuleLoaded(
    std::shared_ptr<bookfiler::FSDB_Interface> module) {
  std::cout << "MainWindow::FSDB_ModuleLoaded()" << std::endl;
  FSDB_Module = module;
}

void MainWindow::fileTreePaneModuleLoaded(
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
  fileTreePaneWidget = fileTreePaneModule->getWidget();
  std::cout << "std::make_shared<RenderWidget>()" << std::endl;
  renderWidget = std::make_shared<RenderWidget>();
  renderWidget->renderFunction =
      std::bind(&bookfiler::FileTreePaneWidget::render, fileTreePaneWidget,
                std::placeholders::_1);
  renderWidget->initGraphicsFunction =
      std::bind(&bookfiler::FileTreePaneWidget::initGraphics,
                fileTreePaneWidget, std::placeholders::_1);
  // renderWidget->renderFunction();
  // needed to prevent crashing on program exit
  centralQWidgetPtrs.push_back(renderWidget);
  // TODO: Check if widgets need to be added on paint cycles
  QSizePolicy policy = renderWidget->sizePolicy();
  policy.setHorizontalStretch(2);
  renderWidget->setSizePolicy(policy);
  ui->horizontalSplitter->addWidget(renderWidget.get());
}

void MainWindow::allModulesLoaded() {
  // Module Hookup
  recognizeModule->setOcrModule(ocrModule);
  recognizeModule->setPdfModule(pdfModule);
  recognizeModel = recognizeModule->newModel();
  /* SIGNALS AND SLOTS HOOKUP
   */
  ui->filesSelectedSignal.connect(
      std::bind(&bookfiler::RecognizeModel::addPaths, recognizeModel.get(),
                std::placeholders::_1));
  ui->listItemActivatedSignal.connect(
      std::bind(&bookfiler::RecognizeModel::requestRecognize,
                recognizeModel.get(), std::placeholders::_1));
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
  recognizeModel->imageUpdateSignal.connect(fileTreePaneWidget->setImageSlot);
  recognizeModel->textUpdateSignal.connect(fileTreePaneWidget->textUpdateSlot);
  fileTreePaneWidget->updateSignal.connect(renderWidget->updateSlot);
}
