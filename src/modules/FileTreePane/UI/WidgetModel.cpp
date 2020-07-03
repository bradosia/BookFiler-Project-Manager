/*
 * @name File Tree Pane Module
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief QT5 Widget for viewing a file tree
 */

/*
   @brief Provides a simple tree model to show how to create and use
  hierarchical models.

  Examples
  https://code.qt.io/cgit/qt/qtbase.git/tree/examples/widgets/itemviews/stardelegate/main.cpp?h=5.14
*/

/* QT 5.13.2
 * License: LGPLv3
 */
#include <QStringList>

// Local Project
#include "WidgetModel.hpp"

/*
 * FileTreePane
 */
namespace FileTreePane {

WidgetModel::WidgetModel(QObject *parent) : QAbstractItemModel(parent) {
  rootItem = new WidgetItem({tr("Name"), tr("Value")});
}

WidgetModel::~WidgetModel() { delete rootItem; }

/* Custom methods
 *
 *
 *
 *
 */

void WidgetModel::addData(std::shared_ptr<rapidjson::Value> data) {
  std::shared_ptr<rapidjson::Value> name =
      std::make_shared<rapidjson::Value>("data");
  setupModelData(name, data, rootItem);
}

void WidgetModel::setupModelData(std::shared_ptr<rapidjson::Value> name,
                                 std::shared_ptr<rapidjson::Value> value,
                                 WidgetItem *parent) {
  std::queue<std::tuple<rapidjson::Value *, rapidjson::Value *, WidgetItem *>>
      valueQueue;
  valueQueue.push({name.get(), value.get(), parent});
  while (!valueQueue.empty()) {
    // Raw pointers because its faster in a loop
    rapidjson::Value *name;
    rapidjson::Value *value;
    WidgetItem *parent;
    std::tie(name, value, parent) = valueQueue.front();
    valueQueue.pop();
    // 0: null
    if (value->GetType() == 0) {
      QVector<QVariant> columnData;
      columnData.reserve(2);
      columnData.push_back(name->GetString());
      columnData.push_back("null");
#if FILE_TREE_PANE_MODEL_JSON_DEBUG
      std::cout << "setupModelData() New Value(name=" << name.GetString()
                << ",value=null)" << std::endl;
#endif
      parent->appendChild(new WidgetItem(columnData, parent));
    }
    // 1: false
    else if (value->GetType() == 1) {
      QVector<QVariant> columnData;
      columnData.reserve(2);
      columnData.push_back(name->GetString());
      columnData.push_back("false");
#if FILE_TREE_PANE_MODEL_JSON_DEBUG
      std::cout << "setupModelData() New Value(name=" << name.GetString()
                << ",value=false)" << std::endl;
#endif
      parent->appendChild(new WidgetItem(columnData, parent));
    }
    // 2: true
    else if (value->GetType() == 2) {
      QVector<QVariant> columnData;
      columnData.reserve(2);
      columnData.push_back(name->GetString());
      columnData.push_back("true");
#if FILE_TREE_PANE_MODEL_JSON_DEBUG
      std::cout << "setupModelData() New Value(name=" << name.GetString()
                << ",value=true)" << std::endl;
#endif
      parent->appendChild(new WidgetItem(columnData, parent));
    }
    // 3: object
    else if (value->GetType() == 3) {
#if FILE_TREE_PANE_MODEL_JSON_DEBUG
      std::cout << "setupModelData() Object(name=" << name.GetString() << ")"
                << std::endl;
#endif
      // new node
      QVector<QVariant> columnData;
      columnData.reserve(2);
      columnData.push_back(name->GetString());
      std::string valueStr;
      valueStr.append("object(").append(")");
      columnData.push_back(valueStr.c_str());
      WidgetItem *parentObject = new WidgetItem(columnData, parent);
      parent->appendChild(parentObject);
      // add children
      for (rapidjson::Value::MemberIterator itr = value->MemberBegin();
           itr != value->MemberEnd(); ++itr) {
        rapidjson::Value *nameTemp = &(itr->name);
        rapidjson::Value *valueTemp = &(itr->value);
        valueQueue.push({nameTemp, valueTemp, parentObject});
      }
    }
    // 4: array
    else if (value->GetType() == 4) {
      // rapidjson uses SizeType instead of size_t.
      rapidjson::SizeType n = value->Size();
      // new node
      QVector<QVariant> columnData;
      columnData.reserve(2);
      columnData.push_back(name->GetString());
      std::string valueStr;
      valueStr.append("array(").append(std::to_string(n)).append(")");
      columnData.push_back(valueStr.c_str());
      WidgetItem *parentArray = new WidgetItem(columnData, parent);
      parent->appendChild(parentArray);
      // add children
      for (rapidjson::SizeType i = 0; i < n; i++) {
        std::string elementStr;
        elementStr.append("element(").append(std::to_string(i)).append(")");
        rapidjson::Value *elementName = new rapidjson::Value();
        elementName->SetString(elementStr.c_str(), elementStr.length());
        valueQueue.push({elementName, &((*value)[i]), parentArray});
      }
    }
    // 5: string
    else if (value->GetType() == 5) {
      QVector<QVariant> columnData;
      columnData.reserve(2);
      columnData.push_back(name->GetString());
      columnData.push_back(value->GetString());
#if FILE_TREE_PANE_MODEL_JSON_DEBUG
      std::cout << "setupModelData() New Value(name=" << name.GetString()
                << ",value=" << value.GetString() << ")" << std::endl;
#endif
      parent->appendChild(new WidgetItem(columnData, parent));
    }
    // 6: number
    else if (value->GetType() == 6) {
#if FILE_TREE_PANE_MODEL_JSON_DEBUG
      std::cout << "setupModelData() try number name=" << name.GetString()
                << std::endl;
#endif
      QVector<QVariant> columnData;
      columnData.reserve(2);
      columnData.push_back(name->GetString());
      if (value->IsInt()) {
        columnData.push_back(value->GetInt());
#if FILE_TREE_PANE_MODEL_JSON_DEBUG
        std::cout << "setupModelData() New Value(name=" << name.GetString()
                  << ",value=" << value.GetInt() << ")" << std::endl;
#endif
      } else if (value->IsDouble()) {
        columnData.push_back(value->GetDouble());
#if FILE_TREE_PANE_MODEL_JSON_DEBUG
        std::cout << "setupModelData() New Value(name=" << name.GetString()
                  << ",value=" << value.GetDouble() << ")" << std::endl;
#endif
      }
      parent->appendChild(new WidgetItem(columnData, parent));
    }
  }
}

/* Base methods for the view
 *
 *
 *
 */

int WidgetModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return static_cast<WidgetItem *>(parent.internalPointer())->columnCount();
  return rootItem->columnCount();
}

QVariant WidgetModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();

  WidgetItem *item = static_cast<WidgetItem *>(index.internalPointer());

  // Normal data display
  if (role == Qt::DisplayRole) {
    return item->data(index.column());
  }
  // Data displayed in the edit box
  else if (role == Qt::EditRole) {
    return item->data(index.column());
  }

  // for all else
  return QVariant();
}

Qt::ItemFlags WidgetModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::NoItemFlags;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable |
         Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

QVariant WidgetModel::headerData(int section, Qt::Orientation orientation,
                                 int role) const {
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return rootItem->data(section);

  return QVariant();
}

QModelIndex WidgetModel::index(int row, int column,
                               const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  WidgetItem *parentItem;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<WidgetItem *>(parent.internalPointer());

  WidgetItem *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  return QModelIndex();
}

QModelIndex WidgetModel::parent(const QModelIndex &index) const {
  if (!index.isValid())
    return QModelIndex();

  WidgetItem *childItem = static_cast<WidgetItem *>(index.internalPointer());
  WidgetItem *parentItem = childItem->parentItem();

  if (parentItem == rootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int WidgetModel::rowCount(const QModelIndex &parent) const {
  WidgetItem *parentItem;
  if (parent.column() > 0)
    return 0;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<WidgetItem *>(parent.internalPointer());

  return parentItem->childCount();
}

bool WidgetModel::setData(const QModelIndex &index, const QVariant &value,
                          int role) {
  if (role) {
    // TODO
  }
  WidgetItem *item = static_cast<WidgetItem *>(index.internalPointer());
  item->setData(index.column(), value);
  return true;
}

Qt::DropActions WidgetModel::supportedDropActions() const {
  return Qt::CopyAction | Qt::MoveAction;
}

bool WidgetModel::removeRows(int row, int count, const QModelIndex &parent) {
  WidgetItem *parentItem = static_cast<WidgetItem *>(parent.internalPointer());
  parentItem->removeChild(row, count);
  return true;
}

void WidgetModel::setPath(std::string newPath) {
  std::cout << "WidgetModel::setPath(" << newPath << ")\n";
  (*getDirectorySignal)(
      newPath, std::bind(&WidgetModel::getData, this, std::placeholders::_1));
}

void WidgetModel::getData(
    std::shared_ptr<FSDB::filesystem::FileTableData> fileTableData) {
  std::cout << "row 0 pathId=" << fileTableData->getRow(0)->pathId << "\n";
}

} // namespace FileTreePane
