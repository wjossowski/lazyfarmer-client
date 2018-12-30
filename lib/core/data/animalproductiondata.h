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

            Q_PROPERTY(int outputProduct                MEMBER m_outputId           NOTIFY dataChanged)
            Q_PROPERTY(QString outputProductName        READ   outputProductName    NOTIFY dataChanged)

            Q_PROPERTY(int totalTime                    MEMBER m_totalTime          NOTIFY dataChanged)
            Q_PROPERTY(int timeLeft                     MEMBER m_timeLeft           NOTIFY dataChanged)
            Q_PROPERTY(int timeToRefeed                 MEMBER m_timeToRefeed       NOTIFY dataChanged)

            Q_PROPERTY(QVariantList inputProductsInfo   MEMBER m_inputProductsInfo  NOTIFY dataChanged)

            Q_PROPERTY(int chosenProductId              MEMBER m_chosenProductId    NOTIFY chosenProductChanged)
            Q_PROPERTY(int feedAmount                   MEMBER m_feedAmount         NOTIFY chosenProductChanged)

        public:
            using Ptr = QSharedPointer<AnimalProductionData>;

            explicit AnimalProductionData (Player *parent = nullptr);
            ~AnimalProductionData() override = default;

            void update(const QVariant &info) override;

            Q_INVOKABLE bool hasChosenProduct() const { return m_chosenProductId != -1; }
            Q_INVOKABLE void setChosenProductId(int chosenProductId);
            QString outputProductName() const;

        signals:
            void dataChanged() const;
            void chosenProductChanged(int chosenId) const;

        private:
            int m_outputId;

            int m_timeLeft;
            int m_timeToRefeed;

            QVariantList m_inputProductsInfo;
            QHash<int, int> m_inputProductsData;

            int m_chosenProductId;

            int m_feedAmount;
        };

    }

}
