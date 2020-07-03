/*
 * @name File Tree Pane Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief QT5 Widget for viewing a file tree
 */

 /*
    @brief A container for items of data supplied by the simple tree model.
*/

// C++
#include <iostream>

// Local Project
#include "WidgetItemDelegate.hpp"

/*
 * FileTreePane
 */
namespace FileTreePane {

WidgetItem::WidgetItem(const QVector<QVariant> &data,
                               WidgetItem *parent)
    : m_itemData(data), m_parentItem(parent) {}

WidgetItem::~WidgetItem() { qDeleteAll(m_childItems); }

void WidgetItem::appendChild(WidgetItem *item) {
  m_childItems.append(item);
}

void WidgetItem::removeChild(int row, int count) {
  m_childItems.remove(row, count);
}

WidgetItem *WidgetItem::child(int row) {
  if (row < 0 || row >= m_childItems.size())
    return nullptr;
  return m_childItems.at(row);
}

int WidgetItem::childCount() const { return m_childItems.count(); }

int WidgetItem::columnCount() const { return m_itemData.count(); }

QVariant WidgetItem::data(int column) const {
  if (column < 0 || column >= m_itemData.size())
    return QVariant();
  return m_itemData.at(column);
}
void WidgetItem::setData(int column, QVariant value) {
  m_itemData[column] = value;
}
WidgetItem *WidgetItem::parentItem() { return m_parentItem; }

int WidgetItem::row() const {
  if (m_parentItem)
    return m_parentItem->m_childItems.indexOf(
        const_cast<WidgetItem *>(this));

  return 0;
}

/* The initializer list is required to keep the widget embedded in the cell
 * or else the widget appears as a new window
 */
WidgetItemEditor::WidgetItemEditor(QWidget *parent)
    : QComboBox(parent) {
  std::cout << "WidgetItemEditor\n";
}

WidgetItemDelegate::WidgetItemDelegate() {
  std::cout << "WidgetItemDelegate\n";
}

QWidget *
WidgetItemDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const {
  std::cout << "createEditor() index.row=" << index.row()
            << " index.column=" << index.column() << "\n";
  if (index.data().canConvert<QString>()) {
    WidgetItemEditor *editor = new WidgetItemEditor(parent);
    const int row = index.row();
    editor->addItem(QString("one in row %1").arg(row));
    editor->addItem(QString("two in row %1").arg(row));
    editor->addItem(QString("three in row %1").arg(row));
    connect(editor, &WidgetItemEditor::editingFinished, this,
            &WidgetItemDelegate::commitAndCloseEditor);
    return editor;
  }
  return QStyledItemDelegate::createEditor(parent, option, index);
}

void WidgetItemDelegate::setEditorData(QWidget *editor,
                                           const QModelIndex &index) const {
  WidgetItemEditor *cb = qobject_cast<WidgetItemEditor *>(editor);
  // get the index of the text in the combobox that matches the current value of
  // the item
  const QString currentText = index.data(Qt::EditRole).toString();
  const int cbIndex = cb->findText(currentText);
  // if it is valid, adjust the combobox
  if (cbIndex >= 0)
    cb->setCurrentIndex(cbIndex);
}

void WidgetItemDelegate::setModelData(QWidget *editor,
                                          QAbstractItemModel *model,
                                          const QModelIndex &index) const {
  WidgetItemEditor *cb = qobject_cast<WidgetItemEditor *>(editor);
  model->setData(index, cb->currentText(), Qt::EditRole);
}

void WidgetItemDelegate::commitAndCloseEditor() {
  WidgetItemEditor *editor = qobject_cast<WidgetItemEditor *>(sender());
  emit commitData(editor);
  emit closeEditor(editor);
}

} // namespace FileTreePane
