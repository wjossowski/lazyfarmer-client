/**
 ** This file is part of the LazyFarmer project.
 ** Copyright 2018 Wojciech Ossowski <w.j.ossowski@gmail.com>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 3 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "storagemodel.h"

#include <QDebug>

using namespace Core;
using namespace Model;

StorageModel::StorageModel(const Data::Storage::Ptr &storage, QObject *parent)
    : QAbstractListModel(parent)
    , m_storage(storage)
{
    connect(storage.data(), &Data::Storage::storageChanged,
            this,           &StorageModel::reload);
}

int StorageModel::rowCount(const QModelIndex &) const
{
    return m_storage->size();
}

QVariant StorageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    auto dataRole = static_cast<StorageData>(role);
    int row = index.row();

    switch (dataRole) {
    case StorageData::Id:       return m_storage->idAtRow(row);
    case StorageData::Name:     return m_storage->amount(row);
    case StorageData::Icon:     return m_storage->amount(row);
    case StorageData::Amount:   return m_storage->amountAtRow(row);
    default:                    return QVariant();
    }
}

QHash<int, QByteArray> StorageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(static_cast<int>(StorageData::Id), "id");
    roles.insert(static_cast<int>(StorageData::Name), "name");
    roles.insert(static_cast<int>(StorageData::Icon), "icon");
    roles.insert(static_cast<int>(StorageData::Amount), "amount");
    return roles;
}

void StorageModel::reload()
{
    qDebug() << "Reloading Storage Model" << m_storage->size();

    beginResetModel();
    endResetModel();
}
