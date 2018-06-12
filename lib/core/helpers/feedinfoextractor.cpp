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

#include "feedinfoextractor.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

#include <QtCore/QVariantMap>

using namespace Extractors;

void FeedInfoExtractor::extractSpecificData()
{
    // Extract feed input data
    QVariantList feedInputInfo;
    const QJsonArray need = m_datablock.value("need").toArray();
    for (const auto &id : need) {
        feedInputInfo.append(QVariantMap({
            { "In", QString::number(id.toInt()) }
        }));
    }

    m_data.insert("FeedInputInfo", feedInputInfo);

    // Extract feed output data
    const QJsonObject output = m_datablock.value("4").toObject();

    int outputId = output.value("1").toInt();
    int outputRemaining = output.value("2").toInt();

    const QVariantMap outputInfo = {
        { "Out", QString::number(outputId) },
        { "Remaining", QString::number(outputRemaining) }
    };

    m_data.insert("FeedOutputInfo", outputInfo);
}
