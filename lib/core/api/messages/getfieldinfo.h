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

#include "apimessage.h"
#include "../apigateway.h"
#include "../../data/common.h"

namespace Core {

    namespace Api {

        namespace Messages {

            class GetFieldInfo : public ApiMessage
            {
                Q_OBJECT

            public:
                using Ptr = QSharedPointer<GetFieldInfo>;

                explicit GetFieldInfo(ApiGateway *gateway,
                                      const Data::BuildingDetails &buildingData = Data::BuildingDetails());

                const QUrl url() const override;
                void handleResponse(QIODevice *reply) override;

                void setBuilding(const Data::BuildingDetails &buildingData) {
                    m_buildingData = buildingData;
                }

            private:
                Data::BuildingDetails m_buildingData;

            };

        }

    }

}
