/*
 * @name File Tree Pane Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief QT5 Widget for viewing a file tree
 */

#ifndef FILE_TREE_PANE_MODEL_H
#define FILE_TREE_PANE_MODEL_H

#define TREE_MODEL_DEBUG 1

// C++
#include <iostream>
#include <memory>
#include <queue>

/* rapidjson v1.1 (2016-8-25)
 * Developed by Tencent
 * License: MITs
 */
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>

/* boost 1.72.0
 * License: Boost Software License (similar to BSD and MIT)
 */
#include <boost/signals2.hpp>

/* QT 5.13.2
 * License: LGPLv3
 */
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

// FilesystemDatabase Module
#include "../../FilesystemDatabase/Interface.hpp"

// Local Project
#include "WidgetItemDelegate.hpp"

// FSDB = FilesystemDatabase
namespace FSDB {
class FileTable;
}

/*
 * FileTreePane
 */
namespace FileTreePane {

/*
 * @brief Provides a simple tree model to show how to create and use
 * hierarchical models.
 */
class WidgetModel : public QAbstractItemModel {
  Q_OBJECT

public:
  explicit WidgetModel(QObject *parent = nullptr);
  ~WidgetModel();

  /* Custom methods
   */
  void addData(std::shared_ptr<rapidjson::Value> data);

  /* Essential QAbstractItemModel methods
   *
   * https://doc.qt.io/qt-5/qabstractitemmodel.html
   * When subclassing QAbstractItemModel, at the very least you must implement
   * index(), parent(), rowCount(), columnCount(), and data().
   * These functions are used in all read-only models, and form the basis of
   * editable models.
   */
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  /* Copy and move operations methods
   */
  Qt::DropActions supportedDropActions() const override;
  bool removeRows(int row, int count,
                  const QModelIndex &parent = QModelIndex()) override;
  /* Other QAbstractItemModel methods
   */
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;
  /* Custom methods
   */
  void setPath(std::string newPath);
  void getData(std::shared_ptr<FSDB::filesystem::FileTableData> fileTableData);
  // SIGNALS
  std::shared_ptr<boost::signals2::signal<void(
      std::string, std::function<void(std::shared_ptr<FSDB::filesystem::FileTableData>)>)>>
      getDirectorySignal;

private:
  /* Custom methods
   */
  void setupModelData(std::shared_ptr<rapidjson::Value> name,
                      std::shared_ptr<rapidjson::Value> value,
                      WidgetItem *parent);
  WidgetItem *rootItem;
};

} // namespace FileTreePane

#endif // FILE_TREE_PANE_MODEL_H
