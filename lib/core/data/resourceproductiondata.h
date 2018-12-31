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

        class ResourceProductionData: public BuildingData
        {
            Q_OBJECT

            Q_PROPERTY(int totalTime                    MEMBER m_totalTime          NOTIFY  outputProductChanged)

            Q_PROPERTY(int outputProduct                MEMBER m_outputProductId    NOTIFY  outputProductChanged)

            Q_PROPERTY(int inputProduct                 MEMBER m_inputProductId     NOTIFY  outputProductChanged)
            Q_PROPERTY(int productAmount                MEMBER m_productAmount      NOTIFY  outputProductChanged)

            Q_PROPERTY(QVariantList inputProductsInfo   MEMBER m_inputProductInfos  NOTIFY  dataChanged)

        public:
            using Ptr = QSharedPointer<ResourceProductionData>;

            explicit ResourceProductionData (Player *parent = nullptr);
            ~ResourceProductionData() override = default;

            void update(const QVariant &info) override;

            Q_INVOKABLE void setOutputProductId(int outputProductId);

        signals:
            void dataChanged() const;
            void outputProductChanged(int chosenId) const;

        private:
            QVariantList m_inputProductInfos;

            int m_outputProductId;

            int m_inputProductId;
            int m_productAmount;

        };

    }

}
