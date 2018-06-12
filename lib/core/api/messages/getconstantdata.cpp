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

#include "getconstantdata.h"
#include "../apigateway.h"

#include "../../helpers/gameinfoextractor.h"

#include <QtNetwork/QNetworkReply>

using namespace Core;
using namespace Core::Api;
using namespace Core::Api::Messages;
using namespace Core::Extractors;

GetConstantData::GetConstantData(ApiGateway *gateway,
                                 const QString &fileUrl)
    : ApiMessage(gateway, MessageType::GetConstantData),
      m_fileUrl(fileUrl)
{

}

const QUrl GetConstantData::url() const
{
    return m_gateway->buildStaticUrl(m_fileUrl);
}

void GetConstantData::handleResponse(QNetworkReply *reply)
{
    const auto extractor = GameInfoExtractor::constantsExtractor(m_gateway->serverDomain());
    extractor->extract(reply->readAll());

    m_gateway->extractGameData();

    emit finished();
}
