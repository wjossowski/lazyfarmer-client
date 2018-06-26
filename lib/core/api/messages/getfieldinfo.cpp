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


#include "getfieldinfo.h"
#include "extractors/fieldinfoextractor.h"

#include <QtCore/QJsonDocument>

using namespace Core;
using namespace Core::Api;
using namespace Core::Api::Messages;
using namespace Core::Extractors;

GetFieldInfo::GetFieldInfo(ApiGateway *gateway,
                           const Data::BuildingDetails &buildingData)
    : ApiMessage(gateway, MessageType::GetFieldInfo),
      m_buildingData(buildingData)
{

}

const QUrl GetFieldInfo::url() const
{
    return m_gateway->buildEndpointAjaxUrl("farm", {
        { "mode", "gardeninit" },
        { "farm", QString::number(m_buildingData.farmId) },
        { "position", QString::number(m_buildingData.positionId) }
    });
}

void GetFieldInfo::handleResponse(QIODevice *reply)
{
    FieldInfoExtractor extractor;
    extractor.extract(reply->readAll());

    emit finished();
}

void GetFieldInfo::setBuilding(const Data::BuildingDetails &buildingData)
{
    m_buildingData = buildingData;
}
