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

#include "iplayerdata.h"

namespace Core {

    class Player;

    namespace Data {

        class Storage : public IPlayerData
        {
            Q_OBJECT

        public:
            using Ptr = QSharedPointer<Storage>;

            explicit Storage(Player *parent = nullptr) : IPlayerData(parent) { }

            int idAtRow(int row) const { return m_products.keys().at(row); }
            int amountAtRow(int row) const { return m_products.values().at(row).first; }
            QString nameAtRow(int row) const { return m_products.values().at(row).second; }

            int amount(int id) const { return m_products.value(id, {0, ""}).first; }
            QString name (int id) const { return m_products.value(id, {0, ""}).second; }
            int size() const { return m_products.size(); }

            void update(const QVariant &info) override;

        signals:
            void storageChanged() const;

        private:
            QMap<int, QPair<int, QString>> m_products;
        };

    }

}
