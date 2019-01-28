/**
 ** This file is part of the LazyFarmer project.
 ** Copyright 2019 Wojciech Ossowski <w.j.ossowski@gmail.com>.
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

#include "data/fieldgrid.h"

#include <QtCore/QAbstractListModel>

namespace Model {

    class FieldModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        enum class FieldRoles {
            Name                = Qt::DisplayRole,
            Id                  = Qt::UserRole,
            FieldNo,
            DoneTimestamp,
            BaseTimestamp,
            IsWatered,
            Size
        };

        explicit FieldModel(Core::Data::FieldGrid::Ptr fieldGrid,
                            QObject *parent = nullptr);
        virtual ~FieldModel() override = default;

        Q_INVOKABLE int maxColumns() const { return Core::Data::FieldGrid::MAX_GRID_COLUMNS; }
        Q_INVOKABLE int maxRows() const { return Core::Data::FieldGrid::MAX_GRID_ROWS; }

        int rowCount(const QModelIndex &) const override {
            return Core::Data::FieldGrid::MAX_GRID_SIZE;
        }

        QVariant data(const QModelIndex &index, int role) const override;
        bool setData(const QModelIndex &index, const QVariant &value, int role) override;

        QHash<int, QByteArray> roleNames() const override;

    private:
        Core::Data::FieldGrid::Ptr m_fieldGrid;
    };

}
