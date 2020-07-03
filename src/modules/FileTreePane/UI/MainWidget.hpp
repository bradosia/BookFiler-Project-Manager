/*
 * @name File Tree Pane Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief QT5 Widget for viewing a file tree
 */

#ifndef FILE_TREE_PANE_MAIN_WIDGET_H
#define FILE_TREE_PANE_MAIN_WIDGET_H

/* QT 5.13.2
 * License: LGPLv3
 */
#include <QWidget>
#include <QTreeView>

/*
 * FileTreePane
 */
namespace FileTreePane {

class MainWidget : public QTreeView {
  Q_OBJECT

public:
  MainWidget(){
      this->setObjectName("File Tree Pane Widget");
  };
  ~MainWidget(){};
};

} // namespace FileTreePane

#endif
// end FILE_TREE_PANE_MAIN_WIDGET_H
