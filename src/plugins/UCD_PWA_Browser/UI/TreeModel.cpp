/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

/*
    @brief Provides a simple tree model to show how to create and use
   hierarchical models.
*/

#include "TreeModel.hpp"
#include "TreeItem.hpp"

// Boost
#include <boost/algorithm/string.hpp>

// QT
#include <QStringList>

TreeModel::TreeModel(QObject *parent) : QAbstractItemModel(parent) {
  rootItem = new TreeItem({tr("Name"), tr("Value")});
}

TreeModel::~TreeModel() { delete rootItem; }

void TreeModel::addData(const std::string &data) {
  std::vector<std::string> result;
  boost::split(result, data, boost::is_any_of("\n"));
  setupModelData(result, rootItem);
}

void TreeModel::addData(const rapidjson::Value &data) {
  rapidjson::Value name("data");
  setupModelData(name, data, rootItem);
}

int TreeModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return static_cast<TreeItem *>(parent.internalPointer())->columnCount();
  return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

  TreeItem *item = static_cast<TreeItem *>(index.internalPointer());

  return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::NoItemFlags;

  return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const {
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return rootItem->data(section);

  return QVariant();
}

QModelIndex TreeModel::index(int row, int column,
                             const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  TreeItem *parentItem;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<TreeItem *>(parent.internalPointer());

  TreeItem *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const {
  if (!index.isValid())
    return QModelIndex();

  TreeItem *childItem = static_cast<TreeItem *>(index.internalPointer());
  TreeItem *parentItem = childItem->parentItem();

  if (parentItem == rootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const {
  TreeItem *parentItem;
  if (parent.column() > 0)
    return 0;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<TreeItem *>(parent.internalPointer());

  return parentItem->childCount();
}

void TreeModel::setupModelData(const std::vector<std::string> &lines,
                               TreeItem *parent) {
  std::vector<TreeItem *> parents;
  std::vector<unsigned int> indentations;
  parents.push_back(parent);
  indentations.push_back(0);

  unsigned int number = 0;

  while (number < lines.size()) {
    unsigned int position = 0;
    while (position < lines[number].length()) {
      if (lines[number].at(position) != ' ')
        break;
      position++;
    }

    std::string lineData = lines[number].substr(position);
    boost::trim(lineData);

    if (!lineData.empty()) {
      // Read the column data from the rest of the line.

      std::vector<std::string> columnStrings;
      boost::split(columnStrings, lineData, boost::is_any_of("\t"));

      QVector<QVariant> columnData;
      columnData.reserve(columnStrings.size());
      for (const std::string &columnString : columnStrings) {
        columnData.push_back(columnString.c_str());
      }

      if (position > indentations.back()) {
        // The last child of the current parent is now the new parent
        // unless the current parent has no children.

        if (parents.back()->childCount() > 0) {
          parents.push_back(parents.back()->child(parents.back()->childCount() - 1));
          indentations.push_back(position);
        }
      } else {
        while (position < indentations.back() && parents.size() > 0) {
          parents.pop_back();
          indentations.pop_back();
        }
      }

      // Append a new item to the current parent's list of children.
      parents.back()->appendChild(new TreeItem(columnData, parents.back()));
    }
    ++number;
  }
}

void TreeModel::setupModelData(const rapidjson::Value &name,
                               const rapidjson::Value &value,
                               TreeItem *parent) {
  // 0: null
  if (value.GetType() == 0) {
    QVector<QVariant> columnData;
    columnData.reserve(2);
    columnData.push_back(name.GetString());
    columnData.push_back("null");
#if TREE_MODEL_DEBUG
    std::cout << "setupModelData() New Value(name=" << name.GetString()
              << ",value=null)" << std::endl;
#endif
    parent->appendChild(new TreeItem(columnData, parent));
  }
  // 1: false
  else if (value.GetType() == 1) {
    QVector<QVariant> columnData;
    columnData.reserve(2);
    columnData.push_back(name.GetString());
    columnData.push_back("false");
#if TREE_MODEL_DEBUG
    std::cout << "setupModelData() New Value(name=" << name.GetString()
              << ",value=false)" << std::endl;
#endif
    parent->appendChild(new TreeItem(columnData, parent));
  }
  // 2: true
  else if (value.GetType() == 2) {
    QVector<QVariant> columnData;
    columnData.reserve(2);
    columnData.push_back(name.GetString());
    columnData.push_back("true");
#if TREE_MODEL_DEBUG
    std::cout << "setupModelData() New Value(name=" << name.GetString()
              << ",value=true)" << std::endl;
#endif
    parent->appendChild(new TreeItem(columnData, parent));
  }
  // 3: object
  else if (value.GetType() == 3) {
#if TREE_MODEL_DEBUG
    std::cout << "setupModelData() Object(name=" << name.GetString() << ")"
              << std::endl;
#endif
    // new node
    QVector<QVariant> columnData;
    columnData.reserve(2);
    columnData.push_back(name.GetString());
    std::string valueStr;
    valueStr.append("object(").append(")");
    columnData.push_back(valueStr.c_str());
    TreeItem *parentObject = new TreeItem(columnData, parent);
    parent->appendChild(parentObject);
    // add children
    for (rapidjson::Value::ConstMemberIterator itr = value.MemberBegin();
         itr != value.MemberEnd(); ++itr) {
      setupModelData(itr->name, itr->value, parentObject);
    }
  }
  // 4: array
  else if (value.GetType() == 4) {
    // rapidjson uses SizeType instead of size_t.
    rapidjson::SizeType n = value.Size();
    // new node
    QVector<QVariant> columnData;
    columnData.reserve(2);
    columnData.push_back(name.GetString());
    std::string valueStr;
    valueStr.append("array(").append(std::to_string(n)).append(")");
    columnData.push_back(valueStr.c_str());
    TreeItem *parentArray = new TreeItem(columnData, parent);
    parent->appendChild(parentArray);
    // add children
    for (rapidjson::SizeType i = 0; i < n; i++) {
      std::string elementStr;
      elementStr.append("element(").append(std::to_string(i)).append(")");
      rapidjson::Value elementName;
      elementName.SetString(elementStr.c_str(), elementStr.length());
      setupModelData(elementName, value[i], parentArray);
    }
  }
  // 5: string
  else if (value.GetType() == 5) {
    QVector<QVariant> columnData;
    columnData.reserve(2);
    columnData.push_back(name.GetString());
    columnData.push_back(value.GetString());
#if TREE_MODEL_DEBUG
    std::cout << "setupModelData() New Value(name=" << name.GetString()
              << ",value=" << value.GetString() << ")" << std::endl;
#endif
    parent->appendChild(new TreeItem(columnData, parent));
  }
  // 6: number
  else if (value.GetType() == 6) {
    QVector<QVariant> columnData;
    columnData.reserve(2);
    columnData.push_back(name.GetString());
    columnData.push_back(value.GetInt());
#if TREE_MODEL_DEBUG
    std::cout << "setupModelData() New Value(name=" << name.GetString()
              << ",value=" << value.GetInt() << ")" << std::endl;
#endif
    parent->appendChild(new TreeItem(columnData, parent));
  }
}
