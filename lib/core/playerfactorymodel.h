/**
 ** This file is part of the LazyFarmer project.
 ** Copyright 2017 Wojciech Ossowski <w.j.ossowski@gmail.com>.
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

#include "player.h"

#include <QtCore/QAbstractListModel>

namespace Core {

    class PlayerFactoryModel : public QAbstractListModel
    {
        Q_OBJECT
        Q_PROPERTY(int size READ size NOTIFY sizeChanged)

    public:

        enum class PlayerRoles {
            Description     = Qt::DisplayRole,
            Level           = Qt::UserRole,
            LevelDescription,
            LevelPercentage,
            Money,
            CurrentJob
        };

        explicit PlayerFactoryModel(QObject *parent = nullptr);
        ~PlayerFactoryModel() override = default;

        int rowCount(const QModelIndex &parent) const override;
        QVariant data(const QModelIndex &index, int role) const override;
        QHash<int, QByteArray> roleNames() const override;


        Q_INVOKABLE QSharedPointer<Player> create();
        Q_INVOKABLE void remove(int i);

        int size() { return m_players.size(); }

    signals:
        void sizeChanged(int size) const;

    private:
        QList<QSharedPointer<Player>> m_players;

    };

}
