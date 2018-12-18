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

#include "buildingdata.h"

namespace Core {

    namespace Data {

        class AnimalProductionData : public BuildingData
        {
            Q_OBJECT


        public:
            using Ptr = QSharedPointer<AnimalProductionData>;

            explicit AnimalProductionData (Player *parent = nullptr);
            ~AnimalProductionData() override = default;

            Q_INVOKABLE void foo();

            void update(const QVariant &info) override;
            QString toString() const override;

            QVariant toVariant() override;
            int totalTime() const override { return m_totalTime; }

        signals:
            void dataChanged() const;

        private:
            int m_outputId;
            int m_totalTime;
            int m_timeLeft;
            int m_timeToRefeed;

        };

    }

}
