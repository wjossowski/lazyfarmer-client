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

#include "datablockextractor.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QJsonArray>

using namespace Core;
using namespace Core::Extractors;

void DatablockExtractor::extract(const QByteArray &content)
{
    if (content.isEmpty()){
        return;
    }

    // Create JSON document
    const auto document = QJsonDocument::fromJson(content);

    if (!fetchDatablock(document)){
        return;
    }

    extractSpecificData();
}

bool DatablockExtractor::fetchDatablock(const QJsonDocument &document)
{
    const auto data = document.object();

    if (data.isEmpty()) {
        return false;
    }

    if (!data.contains("datablock")) {
        return false;
    }

    // Get result
    const QJsonArray datablock = data.value("datablock").toArray();

    // Check if result is correct;
    if (datablock.size() < 2 ||
        datablock.first().toInt() != 1) {
        return false;
    }

    // Get Production Info Object
    m_datablock = datablock.at(1).toObject();
    if (m_datablock.isEmpty()) {
        return false;
    }

    return true;
}
