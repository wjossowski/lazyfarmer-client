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

#pragma once

#include "core/data/storage.h"

#include <QAbstractListModel>

namespace Model {

    class StorageModel : public QAbstractListModel
    {
        Q_OBJECT

    public:

        enum class StorageData {
            Name    = Qt::DisplayRole,
            Icon    = Qt::DecorationRole,
            Id      = Qt::UserRole,
            Amount  = Qt::UserRole + 1
        };

        explicit StorageModel(const Core::Data::Storage::Ptr &storage, QObject *parent = nullptr);
        virtual ~StorageModel() {}

        int rowCount(const QModelIndex &) const override;
        QVariant data(const QModelIndex &index, int role) const override;
        QHash<int, QByteArray> roleNames() const override;

    private slots:
        void reload();

    private:
       Core::Data::Storage::Ptr m_storage;

    };

}
