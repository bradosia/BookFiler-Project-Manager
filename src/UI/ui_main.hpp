/*
 * @name BookFiler Recognize
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief Text recognition viewer, editor, and analyzer.

 * Icons and images owned by their respective owners
 */

#ifndef UI_MAIN_H
#define UI_MAIN_H

// Config
#include "../core/config.hpp"

// C++
#include <iostream>
#include <vector>

/* boost 1.72.0
 * License: Boost Software License (similar to BSD and MIT)
 */
#include <boost/signals2.hpp>

/* QT 5.13.2-1
 * License: LGPLv3
 */
#include <QObject>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <QStyleFactory>
#include <QDebug>

namespace Ui {

class main : public QObject {
  Q_OBJECT
public:
  QAction *actionAbout;
  QAction *actionOpen;
  QWidget *centralwidget;
  QSplitter *horizontalSplitter;
  QHBoxLayout *horizontalLayout;
  QMenuBar *menubar;
  QMenu *menuFile;
  QMenu *menuHelp;
  QStatusBar *statusbar;
  QMainWindow *mainWindow;

  QFileDialog *fileDialog;
  QListWidget *listWidget;

  boost::signals2::signal<void(std::shared_ptr<std::vector<std::string>>)>
      filesSelectedSignal;
  boost::signals2::signal<void(std::string)> listItemActivatedSignal;

  void setupUi(QMainWindow *main) {
    mainWindow = main;

    if (main->objectName().isEmpty())
      main->setObjectName(QString::fromUtf8("main"));
    main->resize(800, 600);
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icons/file/icons8-document.svg"), QSize(),
                 QIcon::Normal, QIcon::Off);
    main->setWindowIcon(icon);
    actionAbout = new QAction(main);
    actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
    actionOpen = new QAction(main);
    actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
    centralwidget = new QWidget(main);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    centralwidget->setAcceptDrops(false);
    /* Splitter
     */
    horizontalSplitter = new QSplitter();
    horizontalSplitter->setOrientation(Qt::Horizontal);
    // Layout needed to expand to full parent
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->addWidget(horizontalSplitter);
    centralwidget->setLayout(horizontalLayout);

    main->setCentralWidget(centralwidget);
    menubar = new QMenuBar(main);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 800, 21));
    menuFile = new QMenu(menubar);
    menuFile->setObjectName(QString::fromUtf8("menuFile"));
    menuHelp = new QMenu(menubar);
    menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
    main->setMenuBar(menubar);
    statusbar = new QStatusBar(main);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    main->setStatusBar(statusbar);

    menubar->addAction(menuFile->menuAction());
    menubar->addAction(menuHelp->menuAction());
    menuFile->addAction(actionOpen);
    menuHelp->addAction(actionAbout);

    /* Side Bar List
     */
    listWidget = new QListWidget();
    listWidget->setAcceptDrops(true);
    QSizePolicy policy = listWidget->sizePolicy();
    policy.setHorizontalStretch(1);
    listWidget->setSizePolicy(policy);
    horizontalSplitter->addWidget(listWidget);

    retranslateUi(main);

    QMetaObject::connectSlotsByName(main);

  } // setupUi

  void retranslateUi(QMainWindow *main) {
    main->setWindowTitle(
        QCoreApplication::translate("main", "BookFiler™ Recognize", nullptr));
    actionAbout->setText(QCoreApplication::translate("main", "About", nullptr));
    actionOpen->setText(QCoreApplication::translate("main", "Open", nullptr));
    menuFile->setTitle(QCoreApplication::translate("main", "File", nullptr));
    menuHelp->setTitle(QCoreApplication::translate("main", "Help", nullptr));
  } // retranslateUi

  void about() {
    std::string message =
        "<h2>Disk Usage Viewer 1.0</h2><p>Author: Branded Lee "
        "</p>"
        "<p>Icons made by <a "
        "href=\"https://icons8.com\" "
        "title=\"ICONS8\">ICONS8</a> and <a "
        "href=\"https://octicons.github.com/\" "
        "title=\"Github\">Github</a></p><p>Style: " +
        std::string(mainWindow->style()->objectName().toStdString()) + "</p>";
    QMessageBox::about(mainWindow, "About Disk Usage Viewer", message.c_str());
  }

  void selectFiles() {
    /* It doesn't seem like there is a way to store the QFileDialog
     * and show it later
     */
    QStringList fileNames;
    QStringList filters;
    std::vector<std::string> vec;
    filters << "Image files (*.png *.xpm *.jpg)"
            << "Text files (*.pdf *.doc *.docx)"
            << "Any files (*)";
    fileDialog = new QFileDialog(mainWindow);
    fileDialog->setNameFilters(filters);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    fileDialog->show();
    connect(fileDialog, &QFileDialog::filesSelected, this,
            &Ui::main::filesSelectedSlot);
  }

  void filesSelectedSlot(const QStringList &selected) {
    std::shared_ptr<std::vector<std::string>> fileSelectedList =
        std::make_shared<std::vector<std::string>>();
    for (int i = 0; i < selected.size(); i++) {
      fileSelectedList->push_back(selected[i].toStdString());
      // Add items to UI List
      QListWidgetItem *newItem = new QListWidgetItem;
      newItem->setText(selected[i]);
      listWidget->addItem(newItem);
    }
#if UI_MAIN_DEBUG
    std::cout << "Ui::main::filesSelectedSlot Selected:\n";
    for (auto a : *fileSelectedList) {
      std::cout << a << "\n";
    }
#endif
    filesSelectedSignal(fileSelectedList);
  }

  void listItemActivated(QListWidgetItem *item) {
#if UI_MAIN_DEBUG
    std::cout << "Ui::main::listItemActivated Activated: "
              << item->text().toStdString() << "\n";
#endif
    listItemActivatedSignal(item->text().toStdString());
  }
};

} // namespace Ui

#endif // UI_MAIN_H
