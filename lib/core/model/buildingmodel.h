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

#include "../data/buildinglist.h"

#include <QtCore/QAbstractListModel>

namespace Model {

    class BuildingModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        using Ptr = QSharedPointer<BuildingModel>;

        enum class BuildingRoles {
            Name        = Qt::DisplayRole,
            Id          = Qt::UserRole,
            Type,
            FarmId,
            Position,
            Level,
            Animals,
            Remaining,
            IsSetUp,
        };

        explicit BuildingModel(const Core::Data::BuildingList::Ptr &buildings,
                               QObject *parent = nullptr);
        ~BuildingModel() override = default;

        int rowCount(const QModelIndex &) const override;
        QVariant data(const QModelIndex &index, int role) const override;
        QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE QVariant at(int row);

        Core::Data::BuildingList::Ptr buildings() const { return m_buildings; }

    private:
        Core::Data::BuildingList::Ptr m_buildings;

    };

}
