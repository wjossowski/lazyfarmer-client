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

#include "../globalgamedata.h"

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QSharedPointer>
#include <QtCore/QDateTime>

namespace Core {

    class Player;

    namespace Data {

        class IPlayerData : public QObject
        {
            Q_OBJECT

        public:
            explicit IPlayerData(Player *parent = nullptr);
            ~IPlayerData() override = default;

            virtual void update(const QVariant &info);
            virtual QString toString() const { return QString(); }
            virtual QVariant toVariant() { return QVariant::fromValue<QObject*>(this); }

            GlobalGameData::Ptr gameData() const;

            Player *owner() const { return m_owner; }
            QDateTime fetchedAt() const { return m_fetchedAt; }

        protected:
            Player *m_owner;
            QDateTime m_fetchedAt;

        };

    }

}
