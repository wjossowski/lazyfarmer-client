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

using namespace Core;
using namespace Core::Extractors;

void FeedInfoExtractor::extractSpecificData()
{
    const QJsonObject buildingObject = m_datablock[m_datablock.keys().first()].toObject();
    const QJsonObject info = buildingObject[buildingObject.keys().first()].toObject();

    // Extract feed input data
    const QJsonObject feed = info["feed"].toObject();
    if (feed.isEmpty()) {
        return;
    }

    QVariantList feedInputInfo;
    for (const auto &id : feed.keys()) {
        const int remaining = feed[id].toObject().value("time").toInt();
        feedInputInfo.append(QVariantMap({
            { "In", id },
            { "Remaining", remaining }
        }));
    }

    m_data.insert("FeedInputInfo", feedInputInfo);

    // Extract feed output data
    int outputId = info.value("pid").toInt(-1);
    int totalTime = info.value("time").toInt(-1);
    int timeLeft = info.value("remain").toInt(-1);
    int timeToRefeed = info.value("rest").toInt(-1);

    m_data.insert("OutputProduct", outputId);
    m_data.insert("TotalTime", totalTime);
    m_data.insert("TimeLeft", timeLeft);
    m_data.insert("TimeToRefeed", timeToRefeed);
}
