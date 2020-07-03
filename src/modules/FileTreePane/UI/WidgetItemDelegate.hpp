/*
 * @name File Tree Pane Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief QT5 Widget for viewing a file tree
 */

#ifndef FILE_TREE_PANE_ITEM_H
#define FILE_TREE_PANE_ITEM_H

/* QT 5.13.2
 * License: LGPLv3
 */
#include <QComboBox>
#include <QStyledItemDelegate>
#include <QVariant>
#include <QVector>

/*
 * FileTreePane
 */
namespace FileTreePane {
/*
 * @brief A container for items of data supplied by the simple tree model.
 */
class WidgetItem {
public:
  explicit WidgetItem(const QVector<QVariant> &data,
                          WidgetItem *parentItem = nullptr);
  ~WidgetItem();

  void appendChild(WidgetItem *child);
  void removeChild(int row, int count);

  WidgetItem *child(int row);
  int childCount() const;
  int columnCount() const;
  QVariant data(int column) const;
  void setData(int column, QVariant value);
  int row() const;
  WidgetItem *parentItem();

private:
  QVector<WidgetItem *> m_childItems;
  QVector<QVariant> m_itemData;
  WidgetItem *m_parentItem;
};

class WidgetItemEditor : public QComboBox {
  Q_OBJECT
signals:
  void editingFinished();

public:
  WidgetItemEditor(QWidget *parent = nullptr);
  ~WidgetItemEditor(){};
};

class WidgetItemDelegate : public QStyledItemDelegate {
  Q_OBJECT
public:
  WidgetItemDelegate();
  ~WidgetItemDelegate() {}

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const;

private:
  void commitAndCloseEditor();
};

} // namespace FileTreePane

#endif // FILE_TREE_PANE_ITEM_H
